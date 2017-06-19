from Helper.pxfilter import XssHtml

def get_only_characters(string):
    only_numbers = ''
    for iterator in string:
        if not str.isdigit(iterator):
            return only_numbers
        else:
            only_numbers = only_numbers + iterator
    return only_numbers


def calculate_key(job):
    key = job.get_title() + ' '

    for iterator in job.get_keywords():
        key = key + iterator + ','

    key = key[:len(key) - 1] + key[len(key):]
    return key


def serialize_job_title_list(list):
    job_titles = ''
    for iterator in list:
        job_titles = job_titles + str(iterator.get_id()) + '#' + iterator.get_title() + '~'

    return job_titles

def serialize_job_list(list):
    all_jobs=''

    for iterator in list:
        keyword_string = ''

        for iterator_keywords in iterator.get_keywords():
            keyword_string = keyword_string + str(iterator_keywords) + ','

        job_titles = str(
            iterator.get_id()) + '#' + iterator.get_title() + '#' + keyword_string + '#' + iterator.get_description()

        all_jobs=all_jobs+job_titles+'~'

    all_jobs = all_jobs[:len(all_jobs) - 1] + all_jobs[len(all_jobs):]

    return all_jobs


def xss_parse(list):
    '''
    Prevent XSS attack
    '''
    return_list = []

    for iterator in list:
        parser = XssHtml()
        parser.feed(iterator)
        parser.close()
        return_list.append(parser.getHtml())

    return return_list