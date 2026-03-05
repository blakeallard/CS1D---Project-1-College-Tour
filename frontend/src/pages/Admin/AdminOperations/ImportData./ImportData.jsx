import BackButton from "@components/BackButton/BackButton"
import HomeButton from "@components/HomeButton/HomeButton";
import { useState } from "react";
import axios from "axios";
import "./ImportData.css";

export default function ImportData() {

    const [selectedFile, setSelectedFile] = useState(null);
    const [status, setStatus] = useState("");

    const handleFileChange = (e) => {
        setSelectedFile(e.target.files[0]);
    };

    const handleImport = async (e) => {
        e.preventDefault();

        if (!selectedFile) {
            alert("Please select a file first.");
            return; }

        const formData = new FormData();
        formData.append("file", selectedFile);

        try {
            setStatus("Uploading...");

            await axios.post("/api/University/import", formData, {
                headers: {
                    "Content-Type": "multipart/form-data"
                }
            });

            setStatus("Import successful!");
            setSelectedFile(null);

        } catch (err) {
            console.error(err);
            setStatus("Import failed.");
        }
    };

    return (
        <>
            <HomeButton />
            <BackButton />

            <header data-aos="zoom-in" data-aos-duration="600">
                <h1>Import Data</h1>
            </header>

            <main data-aos="zoom-in" data-aos-duration="600">
                <form onSubmit={handleImport} className="import-form">

                    <label>
                        Select Database/File:
                        <input
                            type="file"
                            onChange={handleFileChange}
                        />
                    </label>

                    <button type="submit">
                        Import
                    </button>

                    {status && <p className="status">{status}</p>}

                </form>
            </main>
        </>
    );
}
