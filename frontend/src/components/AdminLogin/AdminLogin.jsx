import { useEffect, useState } from "react";
import { useNavigate } from "react-router";
import cross from "../../assets/cross-svgrepo-com.svg";
import "./AdminLogin.css";
import axios from "axios";

export default function AdminLogin({ handleAdminClicked }) {
    let navigate = useNavigate();
    const [username, setUsername] = new useState("");
    const [password, setPassword] = new useState("");

    const exampleUN = "Bilbo Baggins";
    const examplePW = "My Precious";

    const handleUsername = (e) => {
        setUsername(e.target.value);
    }

    const handlePassword = (e) => {
        setPassword(e.target.value);
    }

    const handleSubmit = () => {
        if(username == exampleUN && password == examplePW) {
            window.alert("Success!!!!");
            navigate("/admin")
        } else {
            window.alert("Wth dude u aint an admin");
        }

        setUsername("");
        setPassword("");
    }


    // useEffect(() => {
    //     axios.get("")
    //         .then(response => {
    //             console.log(response.data);
    //         })
    // }, []);
    
    return (
        <div className="admin_login-window">
            <button className="close_window-button" onClick={handleAdminClicked}>
                <img className="close_window-cross" src={cross} alt="close window" />
            </button>
            <h2>Admin Login</h2>
            <div className="admin_login-input_container">
                <div>
                    <label htmlFor="admin-username-input">Username:</label>
                    <input type="text" id="admin-username-input" value={username} className="username-input" onChange={(e) => handleUsername(e)} />
                </div>
                <div>
                    <label htmlFor="admin-password-input">Password:</label>
                    <input type="text" id="admin-password-input" value={password} className="password-input" onChange={(e) => handlePassword(e)} />
                </div>
                <button className="admin_login-submit" onClick={handleSubmit}>Login</button>
            </div>
        </div>
    );
}