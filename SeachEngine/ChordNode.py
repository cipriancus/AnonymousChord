from AnonymousChordClient import AnonymousChordClient
import time
import _pickle as cPickle
from Helper import Helper
import os



class ChordNode():
    def __init__(self, backbone_server_ip):
        self.anonymousChordClient = AnonymousChordClient(backbone_server_ip)
        # self.anonymousChordClient.start_server()

        os.chdir('../../Desktop/SeachEngine/')

        try:
            with open('all_searched.json', 'rb') as handle:
                self.all_searched=cPickle.load(handle)
        except:
            self.all_searched = []

        try:
            with open('all_my_jobs.json', 'rb') as handle:
                self.all_my_jobs=cPickle.load(handle)
        except:
            self.all_my_jobs = []

        try:
            with open('history.json', 'rb') as handle:
                self.history=cPickle.load(handle)
        except:
            self.history = []

        time.sleep(2)

    def get(self, key):
        serialized_job = str(self.anonymousChordClient.get(key),encoding='UTF-8')

        if serialized_job.find('null') ==-1:

            job = cPickle.loads(str(serialized_job))

            self.add_job_searched(job)
            self.save()

            return Helper.serialize_job_list([job])

        else:
            return '0'


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
        self.save()

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
            with open('history.json', 'wb') as handle:
                cPickle.dump(self.history, handle, protocol=cPickle.HIGHEST_PROTOCOL)

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
        self.save()


    def delete_job(self, id):
        for iterator in self.all_my_jobs:
            if iterator.get_id() == id:
                key = Helper.calculate_key(iterator)
                self.delete(key)
                self.all_my_jobs.remove(iterator)
        self.save()

    def get_job(self, id):
        for iterator in self.all_my_jobs:
            if iterator.get_id() == id:
                return iterator

    def get_ip(self):
        return self.anonymousChordClient.getIP()

    def save(self):
        with open('jobs.json', 'wb') as handle:
            cPickle.dump(self.all_my_jobs, handle)

        with open('all_searched.json', 'wb') as handle:
            cPickle.dump(self.all_searched, handle)


if __name__ == '__main__':
    chord = ChordNode('127.0.0.1')
    chord.put('77', '123')




