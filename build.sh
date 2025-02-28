#!/bin/bash
gen_clang_json_file() {
    local cmd=$1
    local ret=""
    if [[ $configed == "true" ]]; then
        rm compile_commands.json
    else
        mv compile_commands.json compile_commands_bk.json
    fi
    bear make -j16
    ret=$?
    if [[ $configed != "true" ]]; then
        jq -s 'add' compile_commands_bk.json compile_commands.json > compile_commands_final.json	 # 实现增量编译
        rm compile_commands_bk.json compile_commands.json
        mv compile_commands_final.json compile_commands.json
    fi
    return $ret
}

drop_rst() {
    sudo iptables -A OUTPUT -p tcp --tcp-flags RST RST -s 192.168.133.140 -d 192.168.133.138 --dport 8080 -j DROP
}

has_c_option="false"
has_r_option="false"
has_k_option="false"
has_g_option="false"    # 启动gdbserver

while getopts ":crkg" opt; do
	case $opt in
		c)
			has_c_option="true"
            has_r_option="true"
			;;
		k)
			has_k_option="true"
			;;
		r)
			has_r_option="true"
			;;
		g)
			has_g_option="true"
			;;
		\?)
			echo "invalid option: -${OPTARG}" >&2
			exit 1
			;;
	esac
done

if [[ $has_k_option == "true" ]]; then
    pid=$(ps -aux | grep 'netstack' | grep -v 'grep' | awk '{print $2}')
    if [ -n "$pid" ]; then
        echo "kill netstack $pid success..."
        echo "hlq" | sudo -S ls > /dev/null 2>&1  
        sudo kill $pid
        sudo -K
        exit 0
    else
        echo "netstack not running..."
    fi
fi

if [[ $has_c_option == "true" ]]; then
    make config
    configed="true"

    if [ $? != 0 ]; then
        echo "[ERROR] config fail."
        exit 1
    fi
fi


make buildInc buildIncRoot="netstack" 
gen_clang_json_file "make -j16"
if [ $? != 0 ]; then
    echo "[ERR] compile fail..."
    exit 1
fi

if [[ $has_r_option == "true" ]]; then
    # echo "hlq" | sudo -S ls > /dev/null 2>&1  
    # $drop_rst
    # sudo make run
    # sudo -K
    make run
fi


if [[ $has_g_option == "true" ]]; then
    # echo "hlq" | sudo -S ls > /dev/null 2>&1  
    # $drop_rst
    # sudo gdbserver :1234 .build/netstack.elf 
    gdbserver :1234 .build/netstack.elf 
fi