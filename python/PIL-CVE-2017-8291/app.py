'''get image size app'''
# coding=utf-8

import os
from flask import Flask, request, redirect, flash, render_template_string, get_flashed_messages
from PIL import Image
from werkzeug.utils import secure_filename

UPLOAD_FOLDER = '/tmp'
ALLOWED_EXTENSIONS = set(['png'])

app = Flask(__name__)
app.config['UPLOAD_FOLDER'] = UPLOAD_FOLDER
app.secret_key = 'test'

def get_img_size(filepath=""):
    '''获取图片长宽'''
    try:
        img = Image.open(filepath)
        img.load()
        return img.size
    except:
        return (0, 0)

def allowed_file(filename):
    '''判断文件后缀是否合法'''
    return '.' in filename and \
           filename.rsplit('.', 1)[1].lower() in ALLOWED_EXTENSIONS

@app.route('/', methods=['GET', 'POST'])
def upload_file():
    '''文件上传app'''
    if request.method == 'POST':
        if 'file' not in request.files:
            flash('No file part')
            return redirect(request.url)
        image_file = request.files['file']
        if image_file.filename == '':
            flash('No selected file')
            return redirect(request.url)
        if not allowed_file(image_file.filename):
            flash('File type don\'t allowed')
            return redirect(request.url) 
        if image_file:
            filename = secure_filename(image_file.filename)
            img_path = os.path.join(app.config['UPLOAD_FOLDER'], filename)
            image_file.save(img_path)
            height, width = get_img_size(img_path)
            return '<html><body>the image\'s height : {}, width : {}; </body></html>'\
                .format(height, width)

    return render_template_string('''
    <!doctype html>
    <title>Upload new File</title>
    <h1>Upload new File</h1>
    {% with messages = get_flashed_messages() %}
    {% if messages %}
        <ul class=flashes>
        {% for message in messages %}
        <li>{{ message }}</li>
        {% endfor %}
        </ul>
    {% endif %}
    {% endwith %}
    <form method=post enctype=multipart/form-data>
      <p><input type=file name=file>
         <input type=submit value=Upload>
    </form>
    ''')

if __name__ == '__main__':
    app.run(threaded=True, port=8000, host="0.0.0.0")
