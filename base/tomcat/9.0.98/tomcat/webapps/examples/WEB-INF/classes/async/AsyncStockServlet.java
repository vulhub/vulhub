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
package async;

import java.io.IOException;
import java.io.PrintWriter;
import java.util.concurrent.ConcurrentLinkedQueue;
import java.util.concurrent.atomic.AtomicInteger;

import javax.servlet.AsyncContext;
import javax.servlet.AsyncEvent;
import javax.servlet.AsyncListener;
import javax.servlet.ServletContext;
import javax.servlet.ServletException;
import javax.servlet.http.HttpServlet;
import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;

import org.apache.juli.logging.Log;
import org.apache.juli.logging.LogFactory;

import async.Stockticker.Stock;
import async.Stockticker.TickListener;

public class AsyncStockServlet extends HttpServlet implements TickListener, AsyncListener{

    private static final long serialVersionUID = 1L;

    private static final Log log = LogFactory.getLog(AsyncStockServlet.class);

    private static final ConcurrentLinkedQueue<AsyncContext> clients =
            new ConcurrentLinkedQueue<>();
    private static final AtomicInteger clientcount = new AtomicInteger(0);

    public AsyncStockServlet() {
        log.info("AsyncStockServlet created");
    }


    @Override
    protected void service(HttpServletRequest req, HttpServletResponse resp)
            throws ServletException, IOException {
        if (req.isAsyncStarted()) {
            req.getAsyncContext().complete();
        } else if (req.isAsyncSupported()) {
            AsyncContext actx = req.startAsync();
            actx.addListener(this);
            resp.setContentType("text/plain");
            clients.add(actx);
            if (clientcount.incrementAndGet()==1) {
                Stockticker ticker = (Stockticker) req.getServletContext().getAttribute(
                        AsyncStockContextListener.STOCK_TICKER_KEY);
                ticker.addTickListener(this);
            }
        } else {
            new Exception("Async Not Supported").printStackTrace();
            resp.sendError(400,"Async is not supported.");
        }
    }


    @Override
    public void tick(Stock stock) {
        for (AsyncContext actx : clients) {
            try {
                writeStock(actx, stock);
            } catch (Exception e) {
                // Ignore. The async error handling will deal with this.
            }
        }
    }


    public void writeStock(AsyncContext actx, Stock stock) throws IOException {
        HttpServletResponse response = (HttpServletResponse)actx.getResponse();
        PrintWriter writer = response.getWriter();
        writer.write("STOCK#");//make client parsing easier
        writer.write(stock.getSymbol());
        writer.write("#");
        writer.write(stock.getValueAsString());
        writer.write("#");
        writer.write(stock.getLastChangeAsString());
        writer.write("#");
        writer.write(String.valueOf(stock.getCnt()));
        writer.write("\n");
        writer.flush();
        response.flushBuffer();
    }


    @Override
    public void shutdown() {
        // The web application is shutting down. Complete any AsyncContexts
        // associated with an active client.
        for (AsyncContext actx : clients) {
            try {
                actx.complete();
            } catch (Exception e) {
                // Ignore. The async error handling will deal with this.
            }
        }
    }


    @Override
    public void onComplete(AsyncEvent event) throws IOException {
        if (clients.remove(event.getAsyncContext()) && clientcount.decrementAndGet()==0) {
            ServletContext sc = event.getAsyncContext().getRequest().getServletContext();
            Stockticker ticker = (Stockticker) sc.getAttribute(
                    AsyncStockContextListener.STOCK_TICKER_KEY);
            ticker.removeTickListener(this);
        }
    }

    @Override
    public void onError(AsyncEvent event) throws IOException {
        event.getAsyncContext().complete();
    }

    @Override
    public void onTimeout(AsyncEvent event) throws IOException {
        event.getAsyncContext().complete();
    }


    @Override
    public void onStartAsync(AsyncEvent event) throws IOException {
        // NOOP
    }
}
