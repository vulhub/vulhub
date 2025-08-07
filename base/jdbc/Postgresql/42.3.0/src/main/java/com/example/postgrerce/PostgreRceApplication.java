package com.example.postgrerce;

import org.springframework.boot.SpringApplication;
import org.springframework.boot.autoconfigure.SpringBootApplication;
import org.springframework.boot.autoconfigure.jdbc.DataSourceAutoConfiguration;

@SpringBootApplication(exclude = {DataSourceAutoConfiguration.class})
public class PostgreRceApplication {

    public static void main(String[] args) {
        SpringApplication.run(PostgreRceApplication.class, args);
    }

}
