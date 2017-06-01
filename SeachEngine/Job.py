import re
import random

def uniqueid():
    seed = random.getrandbits(32)
    while True:
       yield seed
       seed += 1

class Job():
    def __init__(self,title):
        unique_sequence = uniqueid()

        self.Id=next(unique_sequence)
        self.Title=title
        self.Keywords=[]
        self.Description=''
        self.PosterIP=''#define de ip that posted the job

    def add_keyword(self,keyword):
        keyword_lis = re.findall(r"[\w']+", keyword)

        for iterator in keyword_lis:
            self.Keywords.append(str(iterator))

    def add_description(self,description):
        self.Description=description

    def add_poster_ip(self,ip):
        self.PosterIP=ip

    def add_id(self,id):
        self.Id=id

    def get_id(self):
        return self.Id

    def get_title(self):
        return self.Title

    def get_keywords(self):
        return self.Keywords

    def get_description(self):
        return self.Description
