/*
 * Copyright 2015-2016 the original author or authors.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
package example.springdata.rest.headers;

import example.springdata.rest.headers.Customer.Gender;

import javax.annotation.PostConstruct;

import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.boot.SpringApplication;
import org.springframework.boot.autoconfigure.SpringBootApplication;
import org.springframework.data.jpa.repository.config.EnableJpaAuditing;

/**
 * @author Oliver Gierke
 * @soundtrack The Intersphere - Out of phase (Live at Alte Feuerwache Mannheim)
 */
@EnableJpaAuditing
@SpringBootApplication
public class Application {

	public static void main(String... args) {
		SpringApplication.run(Application.class, args);
	}

	@Autowired CustomerRepository customers;

	public @PostConstruct void init() {
		customers.save(new Customer("Dave", "Matthews", Gender.MALE, //
				new Address("4711 Some Place", "54321", "Charlottesville", "VA")));
	}
}
