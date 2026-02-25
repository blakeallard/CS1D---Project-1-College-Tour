import { useState, useEffect } from "react";
import axios from 'axios';
import HomeButton from "../../components/HomeButton/HomeButton";
import cross from "../../assets/cross-svgrepo-com.svg";
import "./Campuses.css";

const SouvenirInfo = ({ handleSvnrClicked, campusName }) => {
    const [souvenirs, setSouvenirs] = new useState([]);
    const campusNameWithoutSpaces = campusName.replaceAll(' ', '%20');

    const handleSouvenirs = (newSvnrs) => {
        setSouvenirs(newSvnrs);
    }

    useEffect(() => {
        axios.get(`/api/University/${campusNameWithoutSpaces}`)
            .then(response => {
                const svnrs = response.data.souvenirs;
                handleSouvenirs(svnrs);
            });
    }, []);

    return (
        <div className="svnr_info-window">
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

export default function Campuses() {
    const [campuses, setCampuses] = new useState([]);
    const [campusName, setCampusName] = new useState("svnr");
    const [svnrClicked, setSvnrClicked] = new useState(false);
    
    const handleCampuses = (campusData) => {
        setCampuses(campusData);
    }

    const handleCampusName = (newCampusName) => {
        setCampusName(newCampusName);
    }
    
    const handleSvnrClicked = () => {
        setSvnrClicked(!svnrClicked);
    }

    useEffect(() => {
        axios.get("/api/University/all")
            .then(response => {
                const allCampuses = response.data.campuses;
                handleCampuses(allCampuses);
            });
    }, []);

    return (
        <>
            <HomeButton />
            <h1>Campus Information</h1>
            <div className="campus_info-container">
                {svnrClicked ?
                    <SouvenirInfo handleSvnrClicked={handleSvnrClicked} campusName={campusName} />
                : ''}
                <table>
                    <thead>
                        <tr>
                            <th>Name</th>
                            <th>Distance from Saddleback</th>
                        </tr>
                    </thead>
                    <tbody>
                        {campuses.map((campus, i) => {
                            return (
                                <tr key={i}>
                                    <td>
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