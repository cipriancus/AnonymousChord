from AnonymousChordClient import AnonymousChordClient
import time
import cPickle
from Helper import Helper

class ChordNode():
    def __init__(self, backbone_server_ip):
        self.anonymousChordClient = AnonymousChordClient(backbone_server_ip)
        self.anonymousChordClient.start_server()

        try:
            self.all_my_jobs = cPickle.load(open("jobs.json", "rb"))
        except:
            self.all_my_jobs = []

        time.sleep(2)

    def get(self, key):
        return self.anonymousChordClient.get(key)

    def put(self, key, value):
        return self.anonymousChordClient.put(key, value)

    def delete(self, key):
        return self.anonymousChordClient.delete(key)

    def anonymous_get(self, key):
        return self.anonymousChordClient.anonymous_get(key)

    def get_connected_nodes(self):
        return self.anonymousChordClient.get_connected_nodes()

    def get_all_my_jobs(self):
        return self.all_my_jobs

    def add_new_job(self, newJob):
        string_job=cPickle.dumps(newJob)
        key=Helper.calculate_key(newJob)

        self.put(key,string_job)
        self.all_my_jobs.append(newJob)

    def delete_job(self, id):
        for iterator in self.all_my_jobs:
            if iterator.get_id() == id:
                key=Helper.calculate_key(iterator)
                self.delete(key)
                self.all_my_jobs.remove(iterator)

    def get_job(self, id):
        for iterator in self.all_my_jobs:
            if iterator.get_id() == id:
                return iterator

    def get_ip(self):
        return self.anonymousChordClient.getIP()

    def __del__(self):
        open("jobs.json", "wt").write(cPickle.dumps(self.all_my_jobs))


if __name__ == '__main__':
    chord = ChordNode('127.0.0.1')
    chord.put('77','123')
