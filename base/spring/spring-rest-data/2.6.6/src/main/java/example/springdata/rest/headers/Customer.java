/*
 * Copyright 2015 the original author or authors.
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

import java.time.LocalDateTime;

import javax.persistence.CascadeType;
import javax.persistence.Entity;
import javax.persistence.EntityListeners;
import javax.persistence.GeneratedValue;
import javax.persistence.Id;
import javax.persistence.OneToOne;
import javax.persistence.Version;

import lombok.Data;
import lombok.RequiredArgsConstructor;

import org.springframework.data.annotation.LastModifiedDate;
import org.springframework.data.jpa.domain.support.AuditingEntityListener;

import com.fasterxml.jackson.annotation.JsonIgnore;

/**
 * Aggregate root representing a customer.
 * 
 * @author Oliver Gierke
 * @soundtrack The Intersphere - Out of phase (Live at Alte Feuerwache Mannheim)
 */
@Entity
@Data
@RequiredArgsConstructor
@EntityListeners(AuditingEntityListener.class)
public class Customer {

	private @GeneratedValue @Id Long id;
	private @Version Long version;
	private @JsonIgnore @LastModifiedDate LocalDateTime lastModifiedDate;

	public final String firstname, lastname;
	public final Gender gender;

	@OneToOne(cascade = CascadeType.ALL, orphanRemoval = true)//
	public final Address address;

	Customer() {
		this.firstname = null;
		this.lastname = null;
		this.address = null;
		this.gender = null;
	}

	static enum Gender {
		MALE, FEMALE;
	}
}
