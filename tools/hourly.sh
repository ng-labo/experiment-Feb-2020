PID=`ps -ef | grep udp-receiver | grep -v grep | awk '{ print $2 }'`
if [ -n "$PID" ]; then
    kill $PID
fi
./udp-receiver > udp-receiver-`date +'%H'`.log 2>/dev/null &
