package org.vulhub.springcloudfunctionsample;

import org.springframework.boot.SpringApplication;
import org.springframework.boot.autoconfigure.SpringBootApplication;
import org.springframework.context.annotation.Bean;
import java.util.function.Function;
import org.springframework.cloud.function.context.FunctionalSpringApplication;
import org.springframework.beans.factory.support.DefaultListableBeanFactory;

@SpringBootApplication
public class SpringCloudFunctionSampleApplication {

    public static void main(String[] args) {
        SpringApplication.run(SpringCloudFunctionSampleApplication.class, args);
    }

    @Bean
    public Function<String, String> uppercase() {
        return value -> value.toUpperCase();
    }

    @Bean
    public Function<String, String> lowercase() {
        return value -> value.toLowerCase();
    }
}
