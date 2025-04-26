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

import lombok.AccessLevel;
import lombok.AllArgsConstructor;
import lombok.EqualsAndHashCode;
import lombok.Getter;
import lombok.experimental.Delegate;

/**
 * A value object to represent {@link Password}s in encrypted and unencrypted state. Note how the methods to create a
 * {@link Password} in encrypted state are restricted to package scope so that only the user subsystem is actually able
 * to encrypted passwords.
 *
 * @author Oliver Gierke
 */
@EqualsAndHashCode
@AllArgsConstructor(access = AccessLevel.PRIVATE)
@Getter(AccessLevel.PACKAGE)
@Embeddable
public class Password implements CharSequence {

	private @Delegate final String password;
	private @Getter transient boolean encrypted;

	Password() {
		this.password = null;
		this.encrypted = true;
	}

	/**
	 * Creates a new raw {@link Password} for the given source {@link String}.
	 *
	 * @param password must not be {@literal null} or empty.
	 * @return
	 */
	public static Password raw(String password) {
		return new Password(password, false);
	}

	/**
	 * Creates a new encrypted {@link Password} for the given {@link String}. Note how this method is package protected so
	 * that encrypted passwords can only created by components in this package and not accidentally by clients using the
	 * type from other packages.
	 *
	 * @param password must not be {@literal null} or empty.
	 * @return
	 */
	static Password encrypted(String password) {
		return new Password(password, true);
	}

	/*
	 * (non-Javadoc)
	 * @see java.lang.Object#toString()
	 */
	public String toString() {
		return encrypted ? password : "********";
	}
}
