import { useState, useEffect } from "react";
import axios from 'axios';
import HomeButton from "../../components/HomeButton/HomeButton";
import cross from "../../assets/cross-svgrepo-com.svg";
import "./Campuses.css";

/** Function for showing souvenir information.
 * 
 * Info is displayed conditionally,
 * only when users click on a campus name.
 */
const SouvenirInfo = ({ handleSvnrClicked, campusName }) => {
    // STATE VARIABLES:
    // Holds souvenir data (array)
    const [souvenirs, setSouvenirs] = new useState([]);

    /** String with campus name, whitespace replaced with '%20'.
     * 
     * Used for grabbing svnr data
     */
    const campusNameWithoutSpaces = campusName.replaceAll(' ', '%20');

    /** Set value for souvenirs to campus-specific souvenirs array */
    const handleSouvenirs = (newSvnrs) => {
        setSouvenirs(newSvnrs);
    }

    // API call for grabbing University-sounvenir data
    useEffect(() => {
        // Actual API call
        axios.get(`/api/University/${campusNameWithoutSpaces}`)
            // Set souvenirs variable to response data
            .then(response => {
                const svnrs = response.data.souvenirs;
                handleSouvenirs(svnrs);
            });
    }, []);

    return (
        <div id="svnr_info-window" data-aos="zoom-in" data-aos-duration="600">
            <button className="close_window-button" onClick={handleSvnrClicked}>
                <img className="close_window-cross" src={cross} alt="close window" />
            </button>
            <h2>Souvenirs for {campusName}</h2>
            <table className="svnr-table">
                <thead>
                    <tr>
                        <th>Item</th>
                        <th>Price</th>
                    </tr>
                </thead>
                <tbody>
                    {/* Map array of svnrs to multiple <td> elements, each containing
                    individual svnr information */}
                    {souvenirs.map((svnr, i) => {
                        return (
                            <tr key={i}>
                                <td>{svnr.item}</td>
                                <td>{svnr.price}</td>
                            </tr>
                        )
                    })}
                </tbody>
            </table>
        </div>
    );
}

/** Campuses Page: shows data for all campuses */
export default function Campuses() {
    // STATE VARIABLES:
    // Holds all campus data (array)
    const [campuses, setCampuses] = new useState([]);
    // Holds current-selected campus name (string)
    const [campusName, setCampusName] = new useState("");
    // Holds whether campus name was clicked or nah (boolean)
    const [svnrClicked, setSvnrClicked] = new useState(false);
    
    /** Set campus to data for all campuses */
    const handleCampuses = (campusData) => {
        setCampuses(campusData);
    }

    /** Set campusName to current campus' name */
    const handleCampusName = (newCampusName) => {
        setCampusName(newCampusName);
    }
    
    /** Toggle snvrClicked to opposite boolean value */
    const handleSvnrClicked = () => {
        setSvnrClicked(!svnrClicked);
    }

    // API call for grabbing all university data
    useEffect(() => {
        // Actual API call for uni data
        axios.get("/api/University/all")
            // Set campuses var to hold response data of all campuses
            .then(response => {
                const allCampuses = response.data.campuses;
                handleCampuses(allCampuses);
            });
    }, []);

    return (
        <>
            <HomeButton />
            {svnrClicked ?
                <SouvenirInfo handleSvnrClicked={handleSvnrClicked} campusName={campusName} />
            : ''}
            <h1 data-aos="zoom-in" data-aos-duration="600">Campus Information</h1>
            <div className="campus_info-container" data-aos="zoom-in" data-aos-duration="600">
                {/* Conditionally show SouvenirInfo:
                if svnrClicked is "true" then show component, otherwise show nothing */}
                <table>
                    <thead>
                        <tr>
                            <th>Name</th>
                            <th>Distance from Saddleback</th>
                        </tr>
                    </thead>
                    <tbody>
                        {/* Map all campuses to <td> elements, each item containing
                        unique campus data */}
                        {campuses.map((campus, i) => {
                            return (
                                <tr key={i}>
                                    <td>
                                        {/* If campus name clicked, toggle svnrClicked and set campusName to selected campus */}
                                        <button className={"souvenir_link"} onClick={() => {
                                            handleSvnrClicked();
                                            handleCampusName(campus.name);
                                        }}>
                                            {campus.name}
                                        </button>
                                    </td>
                                    <td>{campus.distance}</td>
                                </tr>
                            )
                        })}
                    </tbody>
                </table>
            </div>
        </>
    );
}