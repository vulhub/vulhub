import { cookies } from "next/headers";
import Topbar from "../components/topbar";

export default async function Home() {
  const cookieStore = await cookies();
  const username = cookieStore.get("next_username")?.value;

  return (
    <div className="min-vh-100 bg-light">
      <nav className="navbar navbar-expand-lg navbar-light bg-white shadow-sm">
        <div className="container">
          <a className="navbar-brand fw-semibold" href="#">
            Admin Dashboard
          </a>
          <div className="ms-auto">{username && <Topbar />}</div>
        </div>
      </nav>

      <div className="container py-4">
        <div className="row">
          <div className="col-md-12">
            <div className="card border-4 border-light border-opacity-50 bg-white p-4">
              <div className="card-body">
                <h2 className="card-title fs-4 fw-semibold mb-3">
                  Dashboard Content
                </h2>
                <p className="card-text">
                  This is a protected page that only authenticated users can
                  access.
                </p>
                <p className="card-text text-muted mt-2">
                  In a real application, you would display important data and
                  admin controls here.
                </p>
              </div>
            </div>
          </div>
        </div>

        <div className="row mt-4">
          <div className="col-md-4 mb-4">
            <div className="card h-100">
              <div className="card-body">
                <h5 className="card-title">Users</h5>
                <p className="card-text">
                  Manage user accounts and permissions.
                </p>
                <button className="btn btn-outline-primary">
                  Manage Users
                </button>
              </div>
            </div>
          </div>

          <div className="col-md-4 mb-4">
            <div className="card h-100">
              <div className="card-body">
                <h5 className="card-title">Content</h5>
                <p className="card-text">
                  Edit website content and media files.
                </p>
                <button className="btn btn-outline-primary">
                  Edit Content
                </button>
              </div>
            </div>
          </div>

          <div className="col-md-4 mb-4">
            <div className="card h-100">
              <div className="card-body">
                <h5 className="card-title">Settings</h5>
                <p className="card-text">
                  Configure system settings and preferences.
                </p>
                <button className="btn btn-outline-primary">
                  System Settings
                </button>
              </div>
            </div>
          </div>
        </div>
      </div>
    </div>
  );
}
