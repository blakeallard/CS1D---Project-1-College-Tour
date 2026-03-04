import { useEffect, useState } from "react";
import { useNavigate } from "react-router";
import cross from "../../assets/cross-svgrepo-com.svg";
import "./AdminLogin.css";
import axios from "axios";

/** Login component for Admin access.
 *
 * Takes username and password input, validates against credentials
 * stored in database, and handles result accordingly.
 *
 * If successful, navigates user to Admin page. Otherwise, clears
 * form and shows login error.
 */
export default function AdminLogin({ handleAdminClicked }) {
    /** Stores method to force route to Admin page */
    let navigate = useNavigate();
    // DUMMY CREDENTIALS, REPLACE WITH ACTUAL ONES FROM DATABASE
    const exampleUN = "";
    const examplePW = "";

    // STATE VARIABLES:
    // Stores username input
    const [username, setUsername] = new useState("");
    // Stores password input
    const [password, setPassword] = new useState("");

    /** Sets username to user's input for such */
    const handleUsername = (e) => {
        setUsername(e.target.value);
    }

    /** Sets password to user's input for such */
    const handlePassword = (e) => {
        setPassword(e.target.value);
    }

    /** Handles submitted input, accepts or rejects input.
     *
     * Upon success, navigate to Admin page.
     * Upon failure, clear form and show error.
     */
    const handleSubmit = () => {
        if(username == exampleUN && password == examplePW) {
            // window.alert("Success!!!!");
            navigate("/admin")
        } else {
            // window.alert("Wth dude u aint an admin");
        }
        // Clear username & password inputs
        setUsername("");
        setPassword("");
    }

    // API Call for username/password credentials.
    // Used for validating user input
    // useEffect(() => {
    //     axios.get("")
    //         .then(response => {
    //             console.log(response.data);
    //         })
    // }, []);

    return (
        <div id="admin_login-window" data-aos="zoom-in" data-aos-duration="400">
            <button className="close_window-button" onClick={handleAdminClicked}>
                <img className="close_window-cross" src={cross} alt="close window" />
            </button>
            <h2>Admin Login</h2>
            <div className="admin_login-input_container">
                {/* Username Input Field */}
                <div>
                    <label htmlFor="admin-username-input">Username:</label>
                    <input type="text" id="admin-username-input" value={username} className="username-input" onChange={(e) => handleUsername(e)} />
                </div>
                {/* Password Input Field */}
                <div>
                    <label htmlFor="admin-password-input">Password:</label>
                    <input type="password" id="admin-password-input" value={password} className="password-input" onChange={(e) => handlePassword(e)} />
                </div>
                <button className="admin_login-submit" onClick={handleSubmit}>Login</button>
            </div>
        </div>
    );
}
