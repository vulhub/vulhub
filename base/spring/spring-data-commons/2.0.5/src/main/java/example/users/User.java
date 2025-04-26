/*
 * Copyright 2015-2018 the original author or authors.
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
package example.users;

import javax.persistence.Entity;
import javax.persistence.GeneratedValue;
import javax.persistence.Id;
import javax.persistence.PrePersist;
import javax.persistence.PreUpdate;

import lombok.AccessLevel;
import lombok.AllArgsConstructor;
import lombok.EqualsAndHashCode;
import lombok.Getter;
import lombok.RequiredArgsConstructor;

/**
 * A {@link User} domain object. The primary entity of this example. Basically a combination of a {@link Username} and
 * {@link Password}.
 *
 * @author Oliver Gierke
 */
@Entity
@Getter
@RequiredArgsConstructor
@AllArgsConstructor(access = AccessLevel.PACKAGE)
@EqualsAndHashCode(of = "id")
public class User {

	private @GeneratedValue @Id Long id;
	private final Username username;
	private final Password password;

	User() {
		this.username = null;
		this.password = null;
	}

	/**
	 * Makes sure only {@link User}s with encrypted {@link Password} can be persisted.
	 */
	@PrePersist
	@PreUpdate
	void assertEncrypted() {

		if (!password.isEncrypted()) {
			throw new IllegalStateException("Tried to persist/load a user with a non-encrypted password!");
		}
	}
}
