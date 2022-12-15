'''
Script para inciar la interfaz de red tap0
para utilizarlo en gns3
'''

import subprocess, netifaces as nif

interfaces = nif.interfaces()
if 'tap0' not in interfaces:
    print("Creando tap0")
    username = subprocess.check_output("whoami")
    comando = 'sudo tunctl -t tap0 -u ' + username.decode('utf-8')
    crea_tap0 = subprocess.run(comando,shell= True,stdout=subprocess.PIPE,stderr=subprocess.PIPE,text=True)
    print(crea_tap0.stdout)
    if crea_tap0.stderr != '':
        print(crea_tap0.strerr)
        exit()
    print("tap0 creado")
interfaces = nif.interfaces()
print(f'Interfaces de red disponibles: {interfaces}')
inet_interface = input("Selecciona la interfaz con salida a internet: ")
if inet_interface not in interfaces:
    print(f"INTERFAZ {inet_interface} NO DISPONIBLE...")
    exit()

addrs = nif.ifaddresses(inet_interface)
inetf = addrs[nif.AF_INET][0]
print(f'Interfaz: {inet_interface}, Info: {inetf}')
inetf_ip = inetf['addr']
inetf_mask = inetf['netmask']
inetf_mask_num = sum(bin(int(x)).count('1') for x in inetf_mask.split('.'))
tap0_ip = inetf_ip.split('.')
tap0_ip[-1] = str(int(tap0_ip[-1])+1)
tap0_ip = '.'.join(tap0_ip)
print(f'ip tap0: {tap0_ip}/{inetf_mask_num}')
comando = f'sudo ip addr add {tap0_ip}/{inetf_mask_num} dev tap0'
asigna_ip_tap0 = subprocess.run(comando,shell=True,stdout=subprocess.PIPE,stderr=subprocess.PIPE,text=True)
if asigna_ip_tap0.stderr == '':
    print('ip asignada a tap0')
else:
    print(asigna_ip_tap0.stderr)
comando = 'sudo ip link set tap0 up'


inicia_tap0 = subprocess.run(comando,shell=True,stdout=subprocess.PIPE,stderr=subprocess.PIPE,text=True)
if inicia_tap0.stderr=='':
    print('tap0 iniciado')
else:
    print(incia_tap0.stderr)

opt_gns3 = input("¿Desea agregar las redes de gns3? (s/n): ")
if opt_gns3.lower() != 's' :
    exit()
else:
    gns3_networks = ['192.168.1.0/24','192.168.2.0/24','192.168.3.0/24','192.168.4.0/24','192.168.5.0/24']
    print(f'Las redes de gns3 son: {gns3_networks} (por defecto)')
    opt_default = input("¿Desea agregar las rutas por defecto? (s/n): ")
    if opt_default.lower() != 's':
        gns3_networks = []
        while True:
            network = input("Ingresa la red a agregar (ej. 192.168.1.0/24): ")
            if network == '':
                break
            gns3_networks.append(network)

    else:
        for network in gns3_networks:
            comando = f'sudo route add -net {network} dev tap0'
            print(comando)
            add_nets = subprocess.run(comando,shell=True,stdout=subprocess.PIPE,stderr=subprocess.PIPE,text=True)
            if add_nets.stderr=='':
                print(f'{network} agregada')
            else:
                print(add_nets.stderr)
