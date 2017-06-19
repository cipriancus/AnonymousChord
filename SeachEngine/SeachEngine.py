from asyncio.test_utils import dummy_ssl_context

from flask import Flask, render_template, request
from ChordNode import ChordNode
from Job import Job
from Helper import Helper
import os

os.chdir('../../Desktop/SeachEngine/')

chordNode = ChordNode('127.0.0.1')
anonymousFlag = False

app = Flask(__name__)


@app.route('/')
def route_main_page():
    return render_template('index.html')


@app.route('/jobs')
def route_jobs_page():
    return render_template('jobs.html')


@app.route('/history')
def route_history_page():
    return render_template('history.html')


@app.route('/viewjob')
def route_view_job_page():
    global chordNode
    id = request.args.get('id')
    chordNode.add_job_history(int(id))
    return render_template('viewjob.html')


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

    chordNode.delete_job(int(id))
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

    result = ''

    if anonymousFlag == True:
        result = chordNode.anonymous_get(search_string)
    elif anonymousFlag == False:
        result = chordNode.get(search_string)
    else:
        result = 'There has been an error'

    return result


@app.route('/addnewjob', methods=['POST'])
def add_new_job():
    global chordNode
    search_string = request.get_json()

    title = search_string['title']
    keywords = search_string['keywords']
    description = search_string['description']

    title = search_string['title']
    keywords = search_string['keywords']
    description = search_string['description']

    xss_list = Helper.xss_parse([title, keywords, description])

    newJob = Job(xss_list[0])
    newJob.add_description(xss_list[2])
    newJob.add_keyword(xss_list[1])
    newJob.add_poster_ip(chordNode.get_ip())

    chordNode.add_new_job(newJob)

    return 'ok'


@app.route('/submiteditjob', methods=['POST'])
def submit_edit_job():
    global chordNode
    search_string = request.get_json()

    id = search_string['id']
    title = search_string['title']
    keywords = search_string['keywords']
    description = search_string['description']

    xss_list = Helper.xss_parse([id, title, keywords, description])

    newJob = Job(xss_list[1])
    newJob.add_id(int(xss_list[0]))
    newJob.add_description(xss_list[3])
    newJob.add_keyword(xss_list[2])
    newJob.add_poster_ip(chordNode.get_ip())

    chordNode.delete_job(int(id))
    chordNode.add_new_job(newJob)
    return 'ok'


@app.route('/getjobs', methods=['GET'])
def get_all_jobs():
    global chordNode

    return Helper.serialize_job_title_list(chordNode.get_all_my_jobs())


@app.route('/getJobById', methods=['POST'])
def get_job_by_id():
    global chordNode

    id = request.get_json()

    return Helper.serialize_job_list([chordNode.get_job(int(id))])

@app.route('/getHistory', methods=['GET'])
def get_history():
    global chordNode

    return Helper.serialize_job_title_list(chordNode.get_history())

import ssl
context=ssl.SSLContext(ssl.PROTOCOL_TLSv1_2)
context.load_cert_chain('certificate.crt', 'private.key')

if __name__ == '__main__':
    app.run(host='192.168.0.111', port=5000, ssl_context=context)
