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

import javax.servlet.ServletContext;
import javax.servlet.ServletContextEvent;
import javax.servlet.ServletContextListener;

/*
 * Ensures the Stockticker is shut down cleanly when the context stops. This
 * also covers the case when the server shuts down.
 */
public class AsyncStockContextListener implements ServletContextListener {

    public static final String STOCK_TICKER_KEY = "StockTicker";

    @Override
    public void contextInitialized(ServletContextEvent sce) {
        Stockticker stockticker = new Stockticker();
        ServletContext sc = sce.getServletContext();
        sc.setAttribute(STOCK_TICKER_KEY, stockticker);
    }

    @Override
    public void contextDestroyed(ServletContextEvent sce) {
        ServletContext sc = sce.getServletContext();
        Stockticker stockticker = (Stockticker) sc.getAttribute(STOCK_TICKER_KEY);
        stockticker.shutdown();
    }
}
