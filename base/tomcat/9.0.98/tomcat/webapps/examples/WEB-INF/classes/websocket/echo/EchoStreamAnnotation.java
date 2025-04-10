/*
 *  Licensed to the Apache Software Foundation (ASF) under one or more
 *  contributor license agreements.  See the NOTICE file distributed with
 *  this work for additional information regarding copyright ownership.
 *  The ASF licenses this file to You under the Apache License, Version 2.0
 *  (the "License"); you may not use this file except in compliance with
 *  the License.  You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 */
package websocket.echo;

import java.io.IOException;
import java.io.OutputStream;
import java.io.Writer;

import javax.websocket.OnMessage;
import javax.websocket.PongMessage;
import javax.websocket.Session;
import javax.websocket.server.ServerEndpoint;

/**
 * The three annotated echo endpoints can be used to test with Autobahn and
 * the following command "wstest -m fuzzingclient -s servers.json". See the
 * Autobahn documentation for setup and general information.
 */
@ServerEndpoint("/websocket/echoStreamAnnotation")
public class EchoStreamAnnotation {

    Writer writer;
    OutputStream stream;

    @OnMessage
    public void echoTextMessage(Session session, String msg, boolean last)
            throws IOException {
        if (writer == null) {
            writer = session.getBasicRemote().getSendWriter();
        }
        writer.write(msg);
        if (last) {
            writer.close();
            writer = null;
        }
    }

    @OnMessage
    public void echoBinaryMessage(byte[] msg, Session session, boolean last)
            throws IOException {
        if (stream == null) {
            stream = session.getBasicRemote().getSendStream();
        }
        stream.write(msg);
        stream.flush();
        if (last) {
            stream.close();
            stream = null;
        }
    }

    /**
     * Process a received pong. This is a NO-OP.
     *
     * @param pm    Ignored.
     */
    @OnMessage
    public void echoPongMessage(PongMessage pm) {
        // NO-OP
    }
}
