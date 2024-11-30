import sys; # used to get argv
import cgi; # used to parse Mutlipart FormData 
from http.server import HTTPServer, BaseHTTPRequestHandler;
from urllib.parse import urlparse, parse_qsl;
import hclib;
import os;
import sqlite3;

GAME_TIME = 300

conn = sqlite3.connect('chess.db')
cur = conn.cursor()

cur.execute("""
            CREATE TABLE IF NOT EXISTS games (
                GAME_NO INTEGER PRIMARY KEY,
                WHITE_HANDLE VARCHAR(32),
                BLACK_HANDLE VARCHAR(32),
                RESULT VARCHAR(256)
            );
            """)
cur.execute("""
            CREATE TABLE IF NOT EXISTS moves (
                GAME_NO INTEGER,
                TURN_NO INTEGER,
                TURN CHAR(1),
                BOARD TEXT,
                REAL_TIME DATETIME DEFAULT CURRENT_TIMESTAMP,
                WHITE_TIME INTEGER,
                BLACK_TIME INTEGER,
                FOREIGN KEY (GAME_NO) REFERENCES games(GAME_NO),
                PRIMARY KEY (GAME_NO, TURN_NO)
            );
            """)
data = cur.execute("""SELECT * FROM sqlite_master;""")
print(data.fetchone())
cur.close()
conn.commit()
conn.close()


# handler for our web-server - handles both GET and POST requests
class MyHandler( BaseHTTPRequestHandler ):
    def do_GET(self):
        # parse the URL to get the path and form data
        parsed  = urlparse(self.path);

        # check if the web-pages matches the list
        if parsed.path in ['/'] or parsed.path in ['/index.html']:
            fp = open('./client/pages/welcome.html')
            content = fp.read()
            # generate the headers
            self.send_response( 200 ); # OK
            self.send_header( "Content-type", "text/html" );
            self.send_header( "Content-length", len( content ) );
            self.end_headers();

            # send it to the broswer
            self.wfile.write( bytes( content, "utf-8" ) );
        
        elif parsed.path in [ '/css/welcome.css' ]:

            # retreive the HTML file
            fp = open( './client'+self.path );
            content = fp.read();

            # generate the headers
            self.send_response( 200 ); # OK
            self.send_header( "Content-type", "text/css" );
            self.send_header( "Content-length", len( content ) );
            self.end_headers();

            # send it to the broswer
            self.wfile.write( bytes( content, "utf-8" ) );
            
        elif parsed.path in [ '/start.html' ]:

            # retreive the HTML file
            fp = open("./client/pages" + self.path)
            content = fp.read()

            # generate the headers
            self.send_response( 200 ); # OK
            self.send_header( "Content-type", "text/html" );
            self.send_header( "Content-length", len( content ) );
            self.end_headers();

            # send it to the broswer
            self.wfile.write( bytes( content, "utf-8" ) );
        
        # serve chessboard.css
        elif parsed.path in [ '/css/chessboard-1.0.0.css' ]:

            # retreive the HTML file
            fp = open( './client'+self.path );
            content = fp.read();

            # generate the headers
            self.send_response( 200 ); # OK
            self.send_header( "Content-type", "text/css" );
            self.send_header( "Content-length", len( content ) );
            self.end_headers();

            # send it to the broswer
            self.wfile.write( bytes( content, "utf-8" ) );
        
        # serve chessboard.js
        elif parsed.path in [ '/js/chessboard-1.0.0.js' ]:

            # retreive the HTML file
            fp = open( './client'+self.path );
            content = fp.read();

            # generate the headers
            self.send_response( 200 ); # OK
            self.send_header( "Content-type", "application/javascript" );
            self.send_header( "Content-length", len( content ) );
            self.end_headers();

            # send it to the broswer
            self.wfile.write( bytes( content, "utf-8" ) );

        # serve upload.html
        elif parsed.path in [ '/upload.html' ]:

            # retreive the HTML file
            fp = open( './client/pages'+self.path );
            content = fp.read();

            # generate the headers
            self.send_response( 200 ); # OK
            self.send_header( "Content-type", "text/html" );
            self.send_header( "Content-length", len( content ) );
            self.end_headers();

            # send it to the broswer
            self.wfile.write( bytes( content, "utf-8" ) );
            
        # IMAGES FROM START, THIS TOOK SO LONG
        elif parsed.path[:5] == '/img/':
            # retrieve the image file
            fp = open('./client' + self.path, 'rb')
            content = fp.read()

            # generate the headers
            self.send_response(200)  # OK
            self.send_header("Content-type", "image/png")
            self.send_header("Content-length", len(content))
            self.end_headers()

            # send it to the browser
            self.wfile.write(content)
            fp.close()
            
        # Add check_opponent handler to GET section:
        elif parsed.path.startswith('/check_opponent'):
            query = dict(parse_qsl(parsed.query))
            game_no = query.get('game_no')
            
            conn = sqlite3.connect('chess.db')
            cursor = conn.cursor()
            cursor.execute("SELECT BLACK_HANDLE FROM games WHERE GAME_NO = ?", (game_no,))
            black_handle = cursor.fetchone()[0]
            conn.close()
            
            # HOLY I GOT IT
            black_handle_adjusted = "null" if black_handle is None else "\"" + black_handle + "\""
            content = f'{{"black_handle": {black_handle_adjusted}}}'
            
            self.send_response(200)
            self.send_header("Content-type", "application/json")
            self.send_header("Content-length", len(content))
            self.end_headers()
            self.wfile.write(bytes(content, "utf-8"))
        
        elif parsed.path in [ '/js/waiting.js' ]:
            fp = open('./client' + self.path)
            content = fp.read()
            self.send_response(200)
            self.send_header("Content-type", "application/javascript")
            self.send_header("Content-length", len(content))
            self.end_headers()
            self.wfile.write(bytes(content, "utf-8"))

        else:
            # generate 404 for GET requests that aren't the 3 files above
            self.send_response( 404 );
            self.end_headers();
            self.wfile.write( bytes( "404: %s not found" % self.path, "utf-8" ) );


    def do_POST(self):
        parsed = urlparse(self.path)
        
        if parsed.path in [ '/display.html' ]:
            # get the form data
            form = cgi.FieldStorage( fp=self.rfile,
                                     headers=self.headers,
                                     environ = { 'REQUEST_METHOD': 'POST',
                                                 'CONTENT_TYPE': 
                                                   self.headers['Content-Type'],
                                               } 
                                   );
        
            # read file and convert to FEN
            theBoard = form['stringboard.txt'].file.read().decode("utf-8");
            theNewBoard = hclib.boardstring(theBoard);
            fen_string = hclib.fen(theNewBoard, 'w', 'KQkq', '-', 0, 1);
            
            # read the HTML file and insert the form data
            content = f"""
            <!DOCTYPE html>
            <html lang="en">
            <head>
                <meta charset="UTF-8">
                <meta name="viewport" content="width=device-width, initial-scale=1.0">
                <title>Chessboard</title>
                <!-- Chessboard.js CSS -->
                <link rel="stylesheet" href="https://unpkg.com/@chrisoakman/chessboardjs@1.0.0/dist/chessboard-1.0.0.min.css" integrity="sha384-q94+BZtLrkL1/ohfjR8c6L+A6qzNH9R2hBLwyoAfu3i/WCvQjzL2RQJ3uNHDISdU" crossorigin="anonymous">
                <script src="https://code.jquery.com/jquery-3.5.1.min.js" integrity="sha384-ZvpUoO/+PpLXR1lu4jmpXWu80pZlYUAfxl5NsBMWOEPSjUn/6Z/hRTt8+pR6L4N2" crossorigin="anonymous"></script>
                <script src="https://unpkg.com/@chrisoakman/chessboardjs@1.0.0/dist/chessboard-1.0.0.min.js" integrity="sha384-8Vi8VHwn3vjQ9eUHUxex3JSN/NFqUg3QbPyX8kWyb93+8AC/pPWTzj+nHtbC5bxD" crossorigin="anonymous"></script>
            </head>
            <body>
                <div id="myBoard" style="width: 400px"></div>

                <script>
                    var board = Chessboard('myBoard', '{fen_string}');
                </script>
            </body>
            </html>
            """

            # generate the headers
            self.send_response( 200 ); # OK
            self.send_header( "Content-type", "text/html" );
            self.send_header( "Content-length", len( content ) );
            self.end_headers();

            # send it to the browser
            self.wfile.write( bytes( content, "utf-8" ) );
            
        elif parsed.path in [ '/board.html' ]:
            # get the form data
            form = cgi.FieldStorage( fp=self.rfile,
                                     headers=self.headers,
                                     environ = { 'REQUEST_METHOD': 'POST',
                                                 'CONTENT_TYPE': 
                                                   self.headers['Content-Type'],
                                               } 
                                   );
        
            # get turn
            turn = form.getvalue('turn')
            nextturn = 'b' if turn == 'w' else 'w'
            wtime = int(form.getvalue('wtime'))
            btime = int(form.getvalue('btime'))
            
            # read file and convert to FEN
            if 'stringboard.txt' in form:
                if form['stringboard.txt'].filename:
                    # It's a file; read its contents
                    theBoard = form['stringboard.txt'].file.read().decode("utf-8")
                    theNewBoard = hclib.boardstring(theBoard);
                    fen_string = hclib.fen(theNewBoard, turn, 'KQkq', '-', 0, 1);
                else:
                    # It's a FEN string
                    fen_string = form.getvalue('stringboard.txt') + " " + turn + " KQkq - 0 1"
                    
            # read the HTML file and insert the form data
            content = f"""
            <!doctype html>
            <html>
            <head>
                <title>Hostage Chess</title>
                <link rel="stylesheet" href="css/chessboard-1.0.0.css">
            </head>
            <body>
                <!--- Begin Example HTML ------------------------------------------------------>
                <div id="myBoard" style="width: 400px"></div>
                <div style="display: flex; gap: 10px; align-items: center;">
                    <p>White: <a id='wMins'>{wtime // 60}</a>:<a id='wSecs'>{("0" + str(wtime % 60))[-2:]}</a></p>
                    <form action="/board.html" method="post" enctype="multipart/form-data" onsubmit="return onFormSubmit()">
                    
                        <input type="hidden" id="board" name="stringboard.txt">
                        
                        <input type="hidden" id="turn" name="turn" value={nextturn}>
                        
                        <input type="hidden" id="wtime" name="wtime">
                        
                        <input type="hidden" id="btime" name="btime">
                        
                        <input type="submit" value="Done">
                    </form>
                    <p><a id='bMins'>{btime // 60}</a>:<a id='bSecs'>{("0" + str(btime % 60))[-2:]}</a> :Black</p>
                </div>
                <!--- End Example HTML -------------------------------------------------------->
                <script src="http://ajax.googleapis.com/ajax/libs/jquery/3.4.1/jquery.min.js"></script>
                <script src="js/chessboard-1.0.0.js"></script>
                <script>
                    let turn = "{turn}";
                    let wtime = {wtime};
                    let btime = {btime};
                    
                    setInterval(() => {{
                        if (turn === 'w' && wtime > 0){{
                            wtime--;
                            // Set minutes
                            document.getElementById('wMins').innerHTML = Math.floor(wtime / 60);
                            // Set seconds
                            document.getElementById('wSecs').innerHTML = ("0" + (wtime % 60).toString()).slice(-2)
                        }}
                        else if (turn === 'b' && btime > 0) {{
                            btime--;
                            // Set minutes
                            document.getElementById('bMins').innerHTML = Math.floor(btime / 60);
                            // Set seconds
                            document.getElementById('bSecs').innerHTML = ("0" + (btime % 60).toString()).slice(-2)
                        }}
                    }}, 1000)
                
                    var config = {{
                        draggable: true,
                        dropOffBoard: 'snapback', // this is the default
                        position: '{fen_string}'
                    }}
                    var board = Chessboard('myBoard', config)
                    
                    function onFormSubmit() {{
                        var newFen = board.fen();
                        document.getElementById('board').value = newFen;
                        document.getElementById('wtime').value = wtime;
                        document.getElementById('btime').value = btime;
                        return true;
                    }}
                </script>
            </body>
            </html>
            """

            # generate the headers
            self.send_response( 200 ); # OK
            self.send_header( "Content-type", "text/html" );
            self.send_header( "Content-length", len( content ) );
            self.end_headers();

            # send it to the browser
            self.wfile.write( bytes( content, "utf-8" ) );

        elif parsed.path in [ '/login.html' ]:
            # get the form data
            form = cgi.FieldStorage( fp=self.rfile,
                                     headers=self.headers,
                                     environ = { 'REQUEST_METHOD': 'POST',
                                                 'CONTENT_TYPE': 
                                                   self.headers['Content-Type'],
                                               } 
                                   );
        
            # get handle from the form-dataw
            handle = form.getvalue('handle')
            
            # Connect to DB
            conn = sqlite3.connect('chess.db')
            cur = conn.cursor()
            
            # Check for open game
            cur.execute("""SELECT GAME_NO FROM games WHERE BLACK_HANDLE IS NULL LIMIT 1""")
            open_game = cur.fetchone()
            
            # handle no game case
            if not open_game:
                # Gets the "game number one larger than the largest number already in the table"
                cur.execute("""SELECT MAX(GAME_NO) FROM games""")
                max_game = cur.fetchone()[0]
                game_no = 1 if max_game is None else max_game + 1
                
                # Send game # and white handle but NOT black handle and result
                cur.execute(
                    """INSERT INTO games (GAME_NO, WHITE_HANDLE, BLACK_HANDLE) VALUES (?, ?, NULL)""",
                    (game_no, handle)
                )
                
                # content = fp.open('./client/pages/loading.html').read()
                content = f"""
                <!DOCTYPE html>
                <html>

                <head>
                    <title>Loading</title>
                    <script src="http://ajax.googleapis.com/ajax/libs/jquery/3.6.3/jquery.min.js"></script>
                    <script src="https://cdn.tailwindcss.com"></script>
                    <script>
                        window.gameNumber = {game_no};
                    </script>
                    <script src="js/waiting.js"></script>
                </head>

                <body class="bg-[#D9C6B0]">
                    <div class="min-h-screen flex items-center justify-center">
                        <div class="text-center justify-items-center">
                            <div class="flex justify-center">
                                <img src="../img/chesspieces/wikipedia/bN.png" alt="Black Queen" class="animate-bounce drop-shadow-md">
                            </div>
                            <h1 class="text-4xl font-bold text-white my-4 drop-shadow-md animate-pulse">{handle} is waiting for their opponent to connect...</h1>
                            <h2 class="text-2xl font-bold text-gray-100 my-4 drop-shadow-md animate-pulse">Lobby: #{game_no}</h2>
                        </div>
                    </div>
                </body>
                </html>
                """
            else:
                # join existing game
                game_no = open_game[0]
                
                # Update game w/ black player
                cur.execute(
                    "UPDATE games SET BLACK_HANDLE =? WHERE GAME_NO = ?",
                    (handle, game_no)
                )
                
                # Initial board state
                cur.execute("""
                    INSERT INTO moves (GAME_NO, TURN_NO, TURN, BOARD, WHITE_TIME, BLACK_TIME)
                    VALUES (?, 1, 'w', 'rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR', ?, ?)
                """, (game_no, GAME_TIME, GAME_TIME))
                
                # Send redirect page for part 5?
                content = f"""
                <!DOCTYPE html>
                <html>
                <head>
                    <title>Starting Game</title>
                    <script>
                        window.location.href = '/opponent.html?game_no={game_no}&turn_no=1';
                    </script>
                </head>
                <body>
                    <h1>Starting game...</h1>
                </body>
                </html>
                """
    
            # close db
            conn.commit()
            conn.close()

            # response
            self.send_response( 200 ); # OK
            self.send_header( "Content-type", "text/html" );
            self.send_header( "Content-length", len( content ) );
            self.end_headers();
            self.wfile.write( bytes( content, "utf-8" ) );

        else:
            # generate 404 for POST requests that aren't the file above
            self.send_response( 404 );
            self.end_headers();
            self.wfile.write( bytes( "404: %s not found" % self.path, "utf-8" ) );


if __name__ == "__main__":
    httpd = HTTPServer( ( '0.0.0.0', int(sys.argv[1]) ), MyHandler );
    print( "Server listing in port:  ", int(sys.argv[1]) );
    httpd.serve_forever();
