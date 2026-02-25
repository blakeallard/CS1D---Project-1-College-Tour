import { NavLink } from "react-router";
import gradCap from "../../assets/grad-cap-logo.svg";
import "./HomeButton.css";

export default function HomeButton() {
    return (
        <>
            <NavLink className="home-button" to={"/"}>
                <img className="home-button" src={gradCap} alt="home button" />
            </NavLink>
        </>
    );
}