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

import java.util.Optional;

import javax.transaction.Transactional;

import lombok.RequiredArgsConstructor;

import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.data.domain.Page;
import org.springframework.data.domain.Pageable;
import org.springframework.security.crypto.password.PasswordEncoder;
import org.springframework.stereotype.Service;
import org.springframework.util.Assert;

/**
 * Domain service to register {@link User}s in the system.
 *
 * @author Oliver Gierke
 */
@Transactional
@Service
@RequiredArgsConstructor
public class UserManagement {

	private final UserRepository repository;
	private final PasswordEncoder encoder;

	/**
	 * Registers a {@link User} with the given {@link Username} and {@link Password}.
	 *
	 * @param username must not be {@literal null}.
	 * @param password must not be {@literal null}.
	 * @return
	 */
	public User register(Username username, Password password) {

		Assert.notNull(username, "Username must not be null!");
		Assert.notNull(password, "Password must not be null!");

		repository.findByUsername(username).ifPresent(user -> {
			throw new IllegalArgumentException("User with that name already exists!");
		});

		Password encryptedPassword = Password.encrypted(encoder.encode(password));

		return repository.save(new User(username, encryptedPassword));
	}

	/**
	 * Returns a {@link Page} of {@link User} for the given {@link Pageable}.
	 *
	 * @param pageable must not be {@literal null}.
	 * @return
	 */
	public Page<User> findAll(Pageable pageable) {

		Assert.notNull(pageable, "Pageable must not be null!");

		return repository.findAll(pageable);
	}

	/**
	 * Returns the {@link User} with the given {@link Username}.
	 *
	 * @param username must not be {@literal null}.
	 * @return
	 */
	public Optional<User> findByUsername(Username username) {

		Assert.notNull(username, "Username must not be null!");

		return repository.findByUsername(username);
	}
}
