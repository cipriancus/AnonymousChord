import socket

'''
    Server commands:
        exit
        remove(key)
        put(key)
        get(key)
        randomWalk(key)
'''


class AnonymousChordClient():
    def __init(self, backbone_server_ip):
        self.backbone_server_ip = backbone_server_ip
        self.port = '8000'
        self.overlay = 'AnonymousChord'
        self.my_ip = self.getIP()

        self.server_ip='127.0.0.1'
        self.server_port=5757

        #start C++ server with arguments ip port overlayIntifier rootDirectory backbone_server_ip

    def getIP(self):
        s = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
        s.connect(("8.8.8.8", 80))
        ip = s.getsockname()[0]
        s.close()
        return ip

    def get(self,key):
        return ''

    def put(self,key,value):
        return ''

    def delete(self,key):
        return ''

    def anonymous_get(self,key):
        return ''