import { NextResponse } from "next/server";

export function middleware(request) {
  //   // Get the pathname of the request
  //   const { pathname } = request.nextUrl;

  //   // Skip middleware for login page and static assets
  //   if (pathname.startsWith("/login")) {
  //     return NextResponse.next();
  //   }

  // Check if user is authenticated by looking for the auth cookie
  const username = request.cookies.get("next_username")?.value;
  const password = request.cookies.get("next_password")?.value;

  console.log("user authenticated", username, password);
  // User is authenticated, allow access to the requested page
  if (username === "admin" && password === "password") {
    return NextResponse.next();
  } else {
    request.cookies.delete("next_username");
    request.cookies.delete("next_password");
  }

  // If user is not authenticated, redirect to login page
  const loginUrl = new URL("/login", request.url);
  return NextResponse.redirect(loginUrl);
}

export const config = {
  matcher: ["/((?!api|static|.*\\..*|_next|favicon.ico|robots.txt|login).*)"],
};
