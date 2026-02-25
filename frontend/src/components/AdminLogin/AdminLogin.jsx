import cross from "../../assets/cross-svgrepo-com.svg";
import "./AdminLogin.css";

export default function AdminLogin({ handleAdminClicked }) {
    return (
        <div className="admin_login-window">
            <button className="close_window-button" onClick={handleAdminClicked}>
                <img className="close_window-cross" src={cross} alt="close window" />
            </button>
            <h3>Admin Login</h3>
            <p>This is the admin login modal. This is where you login to use the admin methods. Punk.</p>
        </div>
    );
}