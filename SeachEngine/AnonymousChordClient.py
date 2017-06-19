import socket
import subprocess
from threading import Thread
from Helper import Helper
from struct import pack
'''
    Server commands:
        exit
        remove(key)
        put(key,value)
        get(key)
        randomWalk(key)
        getallnodes() ->get all connected nodes
'''


class AnonymousChordClient():
    def __init__(self, backbone_server_ip):
        self.backbone_server_ip = backbone_server_ip
        self.port = '8000'
        self.overlay = 'AnonymousChord'
        self.my_ip = self.getIP()

        self.server_ip = '127.0.0.1'
        self.server_port = 5757

        self.server = CppServerThread(self.backbone_server_ip, self.port, self.overlay, self.my_ip)

    def start_server(self):
        self.server.run()

    def stop_server(self):
        self.server.stop()

    def getIP(self):
        s = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
        s.connect(("8.8.8.8", 80))
        ip = s.getsockname()[0]
        s.close()
        return ip

    def get(self, key):
        s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        s.connect((self.server_ip, self.server_port))
        data = pack('!%ds' % 100, b'get')
        s.send(bytes(data))
        data = pack('!%ds' % 1100, bytes(key,encoding='UTF-8'))
        s.send(bytes(data))
        response = s.recv(5100)
        response = str(response)
        s.close()
        return response

    def put(self, key, value):
        s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        s.connect((self.server_ip, self.server_port))
        data = pack('!%ds' % 100, b'put')
        s.send(bytes(data))
        data = pack('!%ds' % 1100, bytes(key,encoding='UTF-8'))
        s.send(bytes(data))
        data = pack('!%ds' % 5100, value)
        s.send(bytes(data))

    def get_connected_nodes(self):
        s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        s.connect((self.server_ip, self.server_port))
        s.send(bytes(b'getallnodes'))
        no_of_ch = s.recv(8)
        no_of_ch = int(Helper.get_only_characters(no_of_ch))
        all_nodes = s.recv(no_of_ch)
        s.close()
        return str(all_nodes)

    def delete(self, key):
        s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        s.connect((self.server_ip, self.server_port))
        data = pack('!%ds' % 100, b'remove')
        s.send(bytes(data))
        data = pack('!%ds' % 1100, bytes(key,encoding='UTF-8'))
        s.send(bytes(data))
        s.close()

    def anonymous_get(self, key):
        return ''

    def exit(self):
        self.s.send(bytes(b'exit'))


class CppServerThread(Thread):
    def __init__(self, backbone_server_ip, port, overlay, my_ip):
        Thread.__init__(self)
        self.backbone_server_ip = backbone_server_ip
        self.port = port
        self.overlay = overlay
        self.my_ip = my_ip

    def run(self):
        # start C++ server with arguments ip port rootDirectory backbone_server_ip
        self.p = subprocess.Popen(
            ["./ChordFiles/AnonymousChordUseCase/dist/Debug/GNU-Linux/anonymouschordusecase " + self.my_ip +
             " " + str(self.port) + " " + self.backbone_server_ip], shell=True,
            cwd='/home/ciprian/Desktop/SeachEngine/')

    def stop(self):
        self.p.kill()

    def __del__(self):
        self.p.kill()


if __name__ == '__main__':
    x = AnonymousChordClient('192.168.0.105')
    x.start_server()
