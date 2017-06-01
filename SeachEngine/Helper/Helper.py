
def get_only_characters(string):
    only_numbers=''
    for iterator in string:
        if not str.isdigit(iterator):
            return only_numbers
        else:
            only_numbers=only_numbers+iterator
    return only_numbers

def calculate_key(job):
    key = job.get_title() + ' '

    for iterator in job.get_keywords():
        key = key + iterator + ','

    key = key[:len(key)-1] + key[len(key):]
    return key
