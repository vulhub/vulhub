package com.b1ngz.sec.util;

import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;

/**
 * Created by b1ngz on 2018/9/9.
 */
public class ResourceUtils {

    public static String readResourceFileAsStr(String filename) throws IOException {
        BufferedReader br = new BufferedReader(new InputStreamReader(ResourceUtils.class.getResourceAsStream(filename)));
        String line;
        StringBuilder sb = new StringBuilder();
        while ((line = br.readLine()) != null) {
            sb.append(line);
        }

        return sb.toString();
    }
}
