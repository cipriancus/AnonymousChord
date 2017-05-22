from nltk.sem.relextract import descape_entity


class Job():
    def __init__(self,title):
        self.Title=title
        self.Keywords=list
        self.Description=''
        self.PosterIP=''#define de ip that posted the job

    def add_keyword(self,keyword):
        self.Keywords.append(keyword)

    def add_description(self,description):
        self.Description=description

    def set_poster_ip(self,ip):
        self.PosterIP=ip