from AnonymousChordClient import AnonymousChordClient

class ChordNode():
    def __init(self, backbone_server_ip):
        self.anonymousChordClient=AnonymousChordClient(backbone_server_ip)

    def get(self,key):
        return self.anonymousChordClient.get(key)

    def put(self,key,value):
        return self.anonymousChordClient.put(key,value)

    def delete(self,key):
        return self.anonymousChordClient.delete(key)

    def anonymous_get(self,key):
        return self.anonymousChordClient.anonymous_get(key)
