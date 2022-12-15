#Creación de mascara de red para red 192.168.106.0/24

sudo tunctl -u roy -t tap0
sudo ifconfig tap0 192.168.106.254/24 up #Se elige la última IP
echo 		Conectar tap0 en GNS3
