require 'sinatra'
require 'net/ftp'
require 'uri'

get '/' do
    'Use /download?uri=ftp://127.0.0.1:2121/&file=/path/to/file.txt to download a ftp file.'
end

get '/download' do
    content_type 'application/octet-stream'

    begin
        uri = URI.parse(params['uri'])

        ftp = Net::FTP.new 
        ftp.connect(uri.host, uri.port)
        ftp.login(uri.user || 'anonymous', uri.password)
        ftp.getbinaryfile(params['file'])
        ftp.close
    rescue
        return '404 Not Found'
    end

    File.open(params['file'], 'rb') {|f|
        return f.read
    }
end