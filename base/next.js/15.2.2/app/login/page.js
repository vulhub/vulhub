"use client";

import { useState } from "react";
import { useRouter } from "next/navigation";

export default function Login() {
  const [username, setUsername] = useState("");
  const [password, setPassword] = useState("");
  const [error, setError] = useState("");
  const router = useRouter();

  const handleSubmit = async (e) => {
    e.preventDefault();

    // Simple validation
    if (!username || !password) {
      setError("Username and password are required");
      return;
    }

    // Set login cookie (expires in 24 hours)
    document.cookie = `next_username=${username}; path=/; max-age=${
      60 * 60
    }; SameSite=Strict`;
    document.cookie = `next_password=${password}; path=/; max-age=${
      60 * 60
    }; SameSite=Strict`;
    console.log("Cookie set", username, password);
    router.push("/"); // Redirect to dashboard
  };

  return (
    <div className="container-fluid bg-light min-vh-100 d-flex align-items-center justify-content-center">
      <div className="card shadow p-4" style={{ maxWidth: "400px" }}>
        <div className="card-body">
          <h1 className="card-title text-center mb-4 fw-bold">Admin Login</h1>

          {error && (
            <div className="alert alert-danger" role="alert">
              {error}
            </div>
          )}

          <form onSubmit={handleSubmit}>
            <div className="mb-3">
              <label htmlFor="username" className="form-label">
                Username
              </label>
              <input
                id="username"
                type="text"
                className="form-control"
                value={username}
                onChange={(e) => setUsername(e.target.value)}
              />
            </div>

            <div className="mb-4">
              <label htmlFor="password" className="form-label">
                Password
              </label>
              <input
                id="password"
                type="password"
                className="form-control"
                value={password}
                onChange={(e) => setPassword(e.target.value)}
              />
            </div>

            <button type="submit" className="btn btn-primary w-100">
              Login
            </button>
          </form>

          <div className="mt-3 text-center text-muted small">
            <p>Demo credentials: admin / password</p>
          </div>
        </div>
      </div>
    </div>
  );
}
