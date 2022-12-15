#El gateway debe coincidir con el del router en GNS3 conectado a tap0
sudo route add -net 192.168.1.0/24 gw 192.168.106.253 dev tap0
sudo route add -net 192.168.2.0/24 gw 192.168.106.253 dev tap0
sudo route add -net 192.168.3.0/24 gw 192.168.106.253 dev tap0
echo 		Sub redes agregadas a la tabla de enrutamiento
ip route
