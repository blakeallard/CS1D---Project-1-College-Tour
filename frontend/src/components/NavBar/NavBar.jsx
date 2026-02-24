import { NavLink } from "react-router";

export default function NavBar() {
    return (
        <nav>
            <NavLink to={"/"}>Home</NavLink>
            <NavLink to={"/admin"}>Admin</NavLink>
            <NavLink to={"/campuses"}>Campuses</NavLink>
        </nav>
    );
}