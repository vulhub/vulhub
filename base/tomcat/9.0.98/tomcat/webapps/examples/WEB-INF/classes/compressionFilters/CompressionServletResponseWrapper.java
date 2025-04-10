/*
 * Licensed to the Apache Software Foundation (ASF) under one or more
 * contributor license agreements.  See the NOTICE file distributed with
 * this work for additional information regarding copyright ownership.
 * The ASF licenses this file to You under the Apache License, Version 2.0
 * (the "License"); you may not use this file except in compliance with
 * the License.  You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
package compressionFilters;

import java.io.IOException;
import java.io.OutputStreamWriter;
import java.io.PrintWriter;
import java.util.Arrays;
import java.util.HashMap;
import java.util.Map;

import javax.servlet.ServletOutputStream;
import javax.servlet.http.HttpServletResponse;
import javax.servlet.http.HttpServletResponseWrapper;

/**
 * Implementation of <b>HttpServletResponseWrapper</b> that works with
 * the CompressionServletResponseStream implementation..
 *
 * @author Amy Roh
 * @author Dmitri Valdin
 */
public class CompressionServletResponseWrapper
        extends HttpServletResponseWrapper {

    // ----------------------------------------------------- Constructor

    /**
     * Calls the parent constructor which creates a ServletResponse adaptor
     * wrapping the given response object.
     *
     * @param response The response object to be wrapped.
     */
    public CompressionServletResponseWrapper(HttpServletResponse response) {
        super(response);
        origResponse = response;
        if (debug > 1) {
            System.out.println("CompressionServletResponseWrapper constructor gets called");
        }
    }


    // ----------------------------------------------------- Instance Variables

    /**
     * Original response
     */
    protected final HttpServletResponse origResponse;

    /**
     * The ServletOutputStream that has been returned by
     * <code>getOutputStream()</code>, if any.
     */
    protected ServletOutputStream stream = null;


    /**
     * The PrintWriter that has been returned by
     * <code>getWriter()</code>, if any.
     */
    protected PrintWriter writer = null;

    /**
     * The threshold number to compress
     */
    protected int compressionThreshold = 0;

    /**
     * The compression buffer size
     */
    protected int compressionBuffer = 8192;  // 8 KiB default

    /**
     * The mime types to compress
     */
    protected String[] compressionMimeTypes = {"text/html", "text/xml", "text/plain"};

    /**
     * Debug level
     */
    protected int debug = 0;

    /**
     * keeps a copy of all headers set
     */
    private final Map<String,String> headerCopies = new HashMap<>();


    // --------------------------------------------------------- Public Methods


    /**
     * Set threshold number.
     *
     * @param threshold The new compression threshold
     */
    public void setCompressionThreshold(int threshold) {
        if (debug > 1) {
            System.out.println("setCompressionThreshold to " + threshold);
        }
        this.compressionThreshold = threshold;
    }

    /**
     * Set compression buffer.
     *
     * @param buffer New size of buffer to use for compressed output
     */
    public void setCompressionBuffer(int buffer) {
        if (debug > 1) {
            System.out.println("setCompressionBuffer to " + buffer);
        }
        this.compressionBuffer = buffer;
    }

    /**
     * Set compressible mime types.
     *
     * @param mimeTypes The new list of mime types that will be considered for
     *                  compression
     */
    public void setCompressionMimeTypes(String[] mimeTypes) {
        if (debug > 1) {
            System.out.println("setCompressionMimeTypes to " +
                    Arrays.toString(mimeTypes));
        }
        this.compressionMimeTypes = mimeTypes;
    }

    /**
     * Set debug level.
     *
     * @param debug The new debug level
     */
    public void setDebugLevel(int debug) {
        this.debug = debug;
    }


    /**
     * Create and return a ServletOutputStream to write the content
     * associated with this Response.
     *
     * @exception IOException if an input/output error occurs
     *
     * @return A new servlet output stream that compressed any data written to
     *         it
     */
    protected ServletOutputStream createOutputStream() throws IOException {
        if (debug > 1) {
            System.out.println("createOutputStream gets called");
        }

        CompressionResponseStream stream = new CompressionResponseStream(
                this, origResponse.getOutputStream());
        stream.setDebugLevel(debug);
        stream.setCompressionThreshold(compressionThreshold);
        stream.setCompressionBuffer(compressionBuffer);
        stream.setCompressionMimeTypes(compressionMimeTypes);

        return stream;
    }


    /**
     * Finish a response.
     */
    public void finishResponse() {
        try {
            if (writer != null) {
                writer.close();
            } else {
                if (stream != null) {
                    stream.close();
                }
            }
        } catch (IOException e) {
            // Ignore
        }
    }


    // ------------------------------------------------ ServletResponse Methods


    /**
     * Flush the buffer and commit this response.
     *
     * @exception IOException if an input/output error occurs
     */
    @Override
    public void flushBuffer() throws IOException {
        if (debug > 1) {
            System.out.println("flush buffer @ GZipServletResponseWrapper");
        }
        ((CompressionResponseStream)stream).flush();

    }

    /**
     * Return the servlet output stream associated with this Response.
     *
     * @exception IllegalStateException if <code>getWriter</code> has
     *  already been called for this response
     * @exception IOException if an input/output error occurs
     */
    @Override
    public ServletOutputStream getOutputStream() throws IOException {

        if (writer != null) {
            throw new IllegalStateException("getWriter() has already been called for this response");
        }

        if (stream == null) {
            stream = createOutputStream();
        }
        if (debug > 1) {
            System.out.println("stream is set to "+stream+" in getOutputStream");
        }

        return stream;
    }

    /**
     * Return the writer associated with this Response.
     *
     * @exception IllegalStateException if <code>getOutputStream</code> has
     *  already been called for this response
     * @exception IOException if an input/output error occurs
     */
    @Override
    public PrintWriter getWriter() throws IOException {

        if (writer != null) {
            return writer;
        }

        if (stream != null) {
            throw new IllegalStateException("getOutputStream() has already been called for this response");
        }

        stream = createOutputStream();
        if (debug > 1) {
            System.out.println("stream is set to "+stream+" in getWriter");
        }
        String charEnc = origResponse.getCharacterEncoding();
        if (debug > 1) {
            System.out.println("character encoding is " + charEnc);
        }
        writer = new PrintWriter(new OutputStreamWriter(stream, charEnc));

        return writer;
    }

    @Override
    public String getHeader(String name) {
        return headerCopies.get(name);
    }

    @Override
    public void addHeader(String name, String value) {
        if (headerCopies.containsKey(name)) {
            String existingValue = headerCopies.get(name);
            if ((existingValue != null) && (existingValue.length() > 0)) {
                headerCopies.put(name, existingValue + "," + value);
            } else {
                headerCopies.put(name, value);
            }
        } else {
            headerCopies.put(name, value);
        }
        super.addHeader(name, value);
    }


    @Override
    public void setHeader(String name, String value) {
        headerCopies.put(name, value);
        super.setHeader(name, value);
    }
}
