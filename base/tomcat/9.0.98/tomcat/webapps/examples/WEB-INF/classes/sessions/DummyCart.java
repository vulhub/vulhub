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
package sessions;

import java.io.Serializable;
import java.util.Collections;
import java.util.HashSet;
import java.util.Set;

public class DummyCart implements Serializable {

    private static final long serialVersionUID = 1L;

    final Set<Item> items = Collections.synchronizedSet(new HashSet<>());
    int itemId = -1;
    String submit = null;

    public void setItemId(int itemId) {
        this.itemId = itemId;
    }

    public void setSubmit(String s) {
        submit = s;
    }

    private void addItem(int itemId) {
        try {
            items.add(Item.values()[itemId]);
        } catch (ArrayIndexOutOfBoundsException e) {
            // Ignore. Can only happen if user edits URL directly.
        }
    }

    private void removeItem(int itemId) {
        try {
            items.remove(Item.values()[itemId]);
        } catch (ArrayIndexOutOfBoundsException e) {
            // Ignore. Can only happen if user edits URL directly.
        }
    }

    public Item[] getItems() {
        return items.toArray(new Item[0]);
    }

    public void processRequest() {
        // null value for submit - user hit enter instead of clicking on
        // "add" or "remove"
        if (submit == null || submit.equals("add")) {
            addItem(itemId);
        } else if (submit.equals("remove")) {
            removeItem(itemId);
        }

        // reset at the end of the request
        reset();
    }

    // reset
    private void reset() {
        submit = null;
        itemId = -1;
    }
}
