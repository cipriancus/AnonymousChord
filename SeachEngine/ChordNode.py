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
            self.history = cPickle.load(open("history.json", "rb"))
            self.all_searched = cPickle.load(open("all_searched.json", "rb"))
        except:
            self.all_my_jobs = []
            self.history = []
            self.all_searched = []

        time.sleep(2)

    def get(self, key):
        serialized_job = self.anonymousChordClient.get(key)
        job = cPickle.loads(serialized_job)

        self.add_job_searched(job)

        return Helper.serialize_job_title_list([job])

    def put(self, key, value):
        return self.anonymousChordClient.put(key, value)

    def anonymous_get(self, key):
        serialized_job = self.anonymousChordClient.anonymous_get(key)
        job = cPickle.loads(serialized_job)

        self.add_job_searched(job)

        return Helper.serialize_job_title_lists([job])

    def delete(self, key):
        return self.anonymousChordClient.delete(key)

    def get_connected_nodes(self):
        return self.anonymousChordClient.get_connected_nodes()

    def get_all_my_jobs(self):
        return self.all_my_jobs

    def add_job_searched(self, job):
        exists = False

        for iterator in self.all_searched:
            if iterator.get_id() == id:
                exists = True

        if exists == False:
            self.history.append(job)

    def get_history(self):
        return self.history

    def add_job_history(self, id):
        job = self.get_job_from_all(id)

        exists = False

        for iterator in self.history:
            if iterator.get_id() == id:
                exists = True

        if exists == False:
            self.history.append()

    def get_job_from_all(self, id):
        for iterator in self.all_searched:
            if iterator.get_id() == id:
                return iterator

    def get_job_history(self, id):
        for iterator in self.history:
            if iterator.get_id() == id:
                return iterator

    def add_new_job(self, newJob):
        string_job = cPickle.dumps(newJob)
        key = Helper.calculate_key(newJob)

        self.put(key, string_job)
        self.all_my_jobs.append(newJob)

    def delete_job(self, id):
        for iterator in self.all_my_jobs:
            if iterator.get_id() == id:
                key = Helper.calculate_key(iterator)
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
        open("history.json", "wt").write(cPickle.dumps(self.history))
        open("all_searched.json", "wt").write(cPickle.dumps(self.all_searched))


if __name__ == '__main__':
    chord = ChordNode('127.0.0.1')
    chord.put('77', '123')
