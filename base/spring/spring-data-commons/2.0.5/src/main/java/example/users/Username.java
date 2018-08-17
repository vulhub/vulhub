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

import javax.persistence.Embeddable;

import lombok.EqualsAndHashCode;

import org.springframework.util.StringUtils;

/**
 * value object to represent user names.
 *
 * @author Oliver Gierke
 */
@EqualsAndHashCode
@Embeddable
public class Username {

	private final String username;

	Username() {
		this.username = null;
	}

	/**
	 * Creates a new {@link Username}.
	 *
	 * @param username must not be {@literal null} or empty.
	 */
	public Username(String username) {

		if (!StringUtils.hasText(username)) {
			throw new IllegalArgumentException("Invalid username!");
		}

		this.username = username;
	}

	/*
	 * (non-Javadoc)
	 * @see java.lang.Object#toString()
	 */
	@Override
	public String toString() {
		return username;
	}
}
