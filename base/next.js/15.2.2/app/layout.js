import "bootstrap/dist/css/bootstrap.min.css";

export const metadata = {
  title: "Admin Dashboard",
  description: "Admin Dashboard for Next.js",
};

export default function RootLayout({ children }) {
  return (
    <html lang="en" suppressHydrationWarning>
      <body>{children}</body>
    </html>
  );
}
