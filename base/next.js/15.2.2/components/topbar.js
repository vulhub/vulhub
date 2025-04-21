"use client";

import { useEffect, useState } from "react";
import { useRouter } from "next/navigation";

export default function Topbar() {
  const [username, setUsername] = useState("");
  const router = useRouter();

  useEffect(() => {
    // Get username from cookie on client side
    const getCookie = (name) => {
      const value = `; ${document.cookie}`;
      const parts = value.split(`; ${name}=`);
      if (parts.length === 2) return parts.pop().split(";").shift();
    };

    const usernameFromCookie = getCookie("next_username");
    setUsername(usernameFromCookie || "");
  }, []);

  const handleLogout = () => {
    // Delete auth cookies
    document.cookie =
      "next_username=; expires=Thu, 01 Jan 1970 00:00:00 UTC; path=/;";
    document.cookie =
      "next_password=; expires=Thu, 01 Jan 1970 00:00:00 UTC; path=/;";
    router.push("/login");
  };

  return (
    <div className="d-flex align-items-center">
      <span className="me-3">Welcome, {username}</span>
      <button className="btn btn-danger" onClick={handleLogout}>
        Logout
      </button>
    </div>
  );
}
