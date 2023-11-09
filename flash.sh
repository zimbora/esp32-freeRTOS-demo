
app_dir=test
port=cu.usbserial-1140

while [ "$#" -gt 0 ]; do
  case "$1" in
  	-h|--help)
      home_dir="$2"
      echo "args:
      	-p/--port		output port indetification
      	-a/--app_dir		name of folder where project is stored inside app"
      exit 0
      ;;
    -p|--port)
      home_dir="$2"
      echo "home dir set: ${home_dir}"
      shift 2
      ;;
    -a|--app_dir)
      app_dir="$2"
      echo "App dir set: $app_dir"
      shift 2
      ;;
    *)
      echo "Unknown parameter: $1"
      exit 1
      ;;
  esac
done

esptool.py --port /dev/${port} write_flash --flash_mode dio --flash_size 4MB 0x10000 ./images/${app_dir}/esp32-freeRTOS2.ino.bin
