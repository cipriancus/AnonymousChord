from AnonymousChordClient import AnonymousChordClient
import time

class ChordNode():
    def __init__(self, backbone_server_ip):
        self.anonymousChordClient=AnonymousChordClient(backbone_server_ip)
        self.anonymousChordClient.start_server()
        time.sleep(2)

    def get(self,key):
        return self.anonymousChordClient.get(key)

    def put(self,key,value):
        return self.anonymousChordClient.put(key,value)

    def delete(self,key):
        return self.anonymousChordClient.delete(key)

    def anonymous_get(self,key):
        return self.anonymousChordClient.anonymous_get(key)

    def get_connected_nodes(self):
        return self.anonymousChordClient.get_connected_nodes()


if __name__ == '__main__':
    chord=ChordNode('127.0.0.1')
    print(chord.delete('77'))