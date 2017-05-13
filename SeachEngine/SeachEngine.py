from flask import Flask, render_template,request
from ChordNode import ChordNode

app = Flask(__name__)

chordNode=ChordNode('192.168.0.105')
anonymousFlag=True

@app.route('/')
def route_main_page():
    return render_template('index.html')

@app.route('/<path:path>')
def static_proxy(path):
  return app.send_static_file(path)

@app.route('/search',methods=['POST'])
def search():
    global chordNode
    search_string=request.get_json()

    value=''

    if anonymousFlag==True:
        value=chordNode.anonymous_get(search_string)
    elif anonymousFlag==False:
        value=chordNode.get(search_string)
    else:
        value='There has been an error'

    return value

if __name__ == '__main__':
    app.run(host='192.168.0.111',port=5000)
