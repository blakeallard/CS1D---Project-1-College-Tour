import { NavLink } from "react-router";
import { useState } from "react";
import AdminLogin from "../AdminLogin/AdminLogin";
import "./NavButtons.css"

export default function NavButtons() {
    const [adminClicked, setAdminClicked] = new useState(false);

    const handleAdminClicked = () => {
        setAdminClicked(!adminClicked);
    }

    return (
        <nav className="nav_buttons-container">
            <NavLink className="nav-button" to={"/tour"}>Start a Tour</NavLink>
            <NavLink className="nav-button" to={"/campuses"}>View Campuses</NavLink>
            <button className="nav-button" onClick={handleAdminClicked}>Administrator Login</button>
            {adminClicked ? <AdminLogin handleAdminClicked={handleAdminClicked} /> : ""}
        </nav>
    );
}