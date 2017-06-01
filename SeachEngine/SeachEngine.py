from flask import Flask, render_template, request
from ChordNode import ChordNode
from Job import Job
from Helper.pxfilter import XssHtml

chordNode = ChordNode('127.0.0.1')
anonymousFlag = False

app = Flask(__name__)


@app.route('/')
def route_main_page():
    return render_template('index.html')


@app.route('/jobs')
def route_jobs_page():
    return render_template('jobs.html')


@app.route('/addjob')
def route_add_job_page():
    return render_template('addjob.html')


@app.route('/editjob')
def route_edit_job_page():
    return render_template('editjob.html')


@app.route('/<path:path>')
def static_proxy(path):
    return app.send_static_file(path)


@app.route('/setflag', methods=['POST'])
def anonymous_flag():
    global anonymousFlag
    anonymousFlag = request.get_json()
    return str(anonymousFlag)


@app.route('/deletejob', methods=['POST'])
def delete_job():
    global chordNode
    id = request.get_json()

    chordNode.delete_job (int(id))
    return 'ok'


@app.route('/getnodes', methods=['GET'])
def get_nodes():
    # global chordNode
    # return chordNode.get_connected_nodes()
    return 'ok'

@app.route('/search', methods=['POST'])
def search():
    global chordNode
    search_string = request.get_json()

    value = ''

    if anonymousFlag == True:
        value = chordNode.anonymous_get(search_string)
    elif anonymousFlag == False:
        value = chordNode.get(search_string)
    else:
        value = 'There has been an error'

    return value


@app.route('/addnewjob', methods=['POST'])
def add_new_job():
    global chordNode
    search_string = request.get_json()

    title = search_string['title']
    keywords = search_string['keywords']
    description = search_string['description']

    '''
    Prevent XSS attack
    '''
    parser = XssHtml()
    parser.feed(title)
    parser.close()
    title = parser.getHtml()

    parser = XssHtml()
    parser.feed(keywords)
    parser.close()
    keywords = parser.getHtml()

    parser = XssHtml()
    parser.feed(description)
    parser.close()
    description = parser.getHtml()

    newJob = Job(title)
    newJob.add_description(description)
    newJob.add_keyword(keywords)
    newJob.add_poster_ip(chordNode.get_ip())

    chordNode.add_new_job(newJob)
    return 'ok'

@app.route('/submiteditjob', methods=['POST'])
def submit_edit_job():
    global chordNode
    search_string = request.get_json()

    id=search_string['id']
    title = search_string['title']
    keywords = search_string['keywords']
    description = search_string['description']

    '''
    Prevent XSS attack
    '''
    parser = XssHtml()
    parser.feed(id)
    parser.close()
    id = parser.getHtml()

    parser = XssHtml()
    parser.feed(title)
    parser.close()
    title = parser.getHtml()

    parser = XssHtml()
    parser.feed(keywords)
    parser.close()
    keywords = parser.getHtml()

    parser = XssHtml()
    parser.feed(description)
    parser.close()
    description = parser.getHtml()

    newJob = Job(title)
    newJob.add_id(int(id))
    newJob.add_description(description)
    newJob.add_keyword(keywords)
    newJob.add_poster_ip(chordNode.get_ip())

    chordNode.delete_job(int(id))
    chordNode.add_new_job(newJob)
    return 'ok'


@app.route('/getjobs', methods=['GET'])
def get_all_jobs():
    global chordNode
    job_titles = ''
    for iterator in chordNode.get_all_my_jobs():
        job_titles = job_titles + str(iterator.get_id()) + '#' + iterator.get_title() + '~'

    return job_titles


@app.route('/getJobById', methods=['POST'])
def get_job_by_id():
    global chordNode

    id = request.get_json()

    selected_job = chordNode.get_job(int(id))

    keyword_string=''

    for iterator in selected_job.get_keywords():
        keyword_string=keyword_string+str(iterator)+','

    job_titles =str(selected_job.get_id()) + '#' + selected_job.get_title() + '#'+keyword_string+'#'+selected_job.get_description()

    return job_titles

if __name__ == '__main__':
    app.run(host='192.168.0.111', port=5000)
