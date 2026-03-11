import { useState, useEffect } from "react";
import axios from "axios";
import HomeButton from "../../../../components/HomeButton/HomeButton";
import "./ASUStart.css";

/**
 * Tour that starts from Arizona State University
 * Uses number input to automatically select N nearest campuses
 */
export default function ASUStart() {
    const [name, setName] = useState("");
    const [numberOfCampuses, setNumberOfCampuses] = useState(3);
    const [maxCampuses, setMaxCampuses] = useState(11);
    const [loading, setLoading] = useState(true);
    const [calculating, setCalculating] = useState(false);
    const [error, setError] = useState("");
    const [tourResult, setTourResult] = useState(null);
    
    // Souvenir state
    const [souvenirs, setSouvenirs] = useState({});
    const [loadingSouvenirs, setLoadingSouvenirs] = useState(false);
    const [purchases, setPurchases] = useState({});
    const [savingPurchases, setSavingPurchases] = useState(false);

    // Load max available campuses
    useEffect(() => {
        const fetchCampuses = async () => {
            try {
                setLoading(true);
                const response = await axios.get("/api/University/from-Arizona State University");
                const campuses = response.data.campuses || [];
                setMaxCampuses(campuses.length + 1); // +1 for ASU itself
                setError("");
            } catch (err) {
                console.error("Error fetching campuses:", err);
                setError("Failed to load campus data. Please try again.");
            } finally {
                setLoading(false);
            }
        };

        fetchCampuses();
    }, []);

    // Load souvenirs for each campus in the tour
    useEffect(() => {
        if (tourResult && tourResult.route) {
            const fetchSouvenirs = async () => {
                setLoadingSouvenirs(true);
                const souvenirData = {};
                
                try {
                    for (const stop of tourResult.route) {
                        const response = await axios.get(`/api/Souvenir/${encodeURIComponent(stop.campus)}`);
                        souvenirData[stop.campus] = response.data;
                    }
                    setSouvenirs(souvenirData);
                } catch (err) {
                    console.error("Error fetching souvenirs:", err);
                    setError("Failed to load souvenirs");
                } finally {
                    setLoadingSouvenirs(false);
                }
            };
            
            fetchSouvenirs();
        }
    }, [tourResult]);

    const handleQuantityChange = (campus, itemName, quantity) => {
        setPurchases(prev => ({
            ...prev,
            [campus]: {
                ...prev[campus],
                [itemName]: Math.max(0, parseInt(quantity) || 0)
            }
        }));
    };

    const handleSubmit = async (e) => {
        e.preventDefault();
        
        if (!name.trim()) {
            setError("Please enter your name");
            return;
        }
        
        if (numberOfCampuses < 1 || numberOfCampuses > maxCampuses) {
            setError(`Please enter a number between 1 and ${maxCampuses}`);
            return;
        }

        try {
            setCalculating(true);
            setError("");
            setTourResult(null);
            setPurchases({});
            
            // Call backend to calculate tour with N nearest campuses
            const response = await axios.post("/api/Tour/calculate-n", {
                startCampus: "Arizona State University",
                numberOfCampuses: numberOfCampuses
            });
            
            if (response.data.success) {
                setTourResult(response.data);
            }
        } catch (err) {
            console.error("Error calculating tour:", err);
            setError("Failed to calculate tour. Please try again.");
        } finally {
            setCalculating(false);
        }
    };
    
    const handleSavePurchases = async () => {
        try {
            setSavingPurchases(true);
            
            const purchaseItems = [];
            for (const campus in purchases) {
                for (const itemName in purchases[campus]) {
                    const quantity = purchases[campus][itemName];
                    if (quantity > 0) {
                        const souvenir = souvenirs[campus]?.find(s => s.name === itemName);
                        if (souvenir) {
                            purchaseItems.push({
                                campus,
                                item: itemName,
                                price: souvenir.price,
                                quantity
                            });
                        }
                    }
                }
            }
            
            if (purchaseItems.length === 0) {
                setError("No items selected for purchase");
                return;
            }
            
            await axios.post("/api/Purchase/save", {
                tourName: "ASU Tour",
                purchases: purchaseItems
            });
            
            alert(`Purchases saved successfully! Total: $${calculateGrandTotal().toFixed(2)}`);
            
            const registration = {
                name,
                tourType: "ASU Tour",
                numberOfCampuses,
                tourResult,
                purchases: purchaseItems,
                date: new Date().toISOString()
            };
            
            const existingRegistrations = JSON.parse(
                localStorage.getItem('asuTourRegistrations') || '[]'
            );
            existingRegistrations.push(registration);
            localStorage.setItem('asuTourRegistrations', JSON.stringify(existingRegistrations));
            
        } catch (err) {
            console.error("Error saving purchases:", err);
            setError("Failed to save purchases");
        } finally {
            setSavingPurchases(false);
        }
    };
    
    const handleReset = () => {
        setName("");
        setNumberOfCampuses(3);
        setTourResult(null);
        setPurchases({});
        setSouvenirs({});
        setError("");
    };
    
    const calculateCampusTotal = (campus) => {
        if (!purchases[campus] || !souvenirs[campus]) return 0;
        
        let total = 0;
        for (const itemName in purchases[campus]) {
            const quantity = purchases[campus][itemName];
            const souvenir = souvenirs[campus].find(s => s.name === itemName);
            if (souvenir && quantity > 0) {
                total += souvenir.price * quantity;
            }
        }
        return total;
    };
    
    const calculateGrandTotal = () => {
        let grandTotal = 0;
        for (const campus in purchases) {
            grandTotal += calculateCampusTotal(campus);
        }
        return grandTotal;
    };

    return (
        <>
            <HomeButton />
            <header data-aos="zoom-in" data-aos-duration="600">
                <h1 className="tours-header">ASU Tour</h1>
            </header>
            <main data-aos="zoom-in" data-aos-duration="600">
                {!tourResult ? (
                    <div className="tour-form-container">
                        {error && <div className="error-message">{error}</div>}
                        
                        <form onSubmit={handleSubmit}>
                            <div className="form-group">
                                <label htmlFor="name">Your Name:</label>
                                <input
                                    id="name"
                                    type="text"
                                    name="name"
                                    value={name}
                                    onChange={(e) => setName(e.target.value)}
                                    placeholder="Enter your name"
                                    disabled={calculating}
                                />
                            </div>

                            <div className="form-group">
                                <label htmlFor="numCampuses">Number of Colleges to Visit:</label>
                                {loading ? (
                                    <p>Loading...</p>
                                ) : (
                                    <>
                                        <input
                                            id="numCampuses"
                                            type="number"
                                            min="1"
                                            max={maxCampuses}
                                            value={numberOfCampuses}
                                            onChange={(e) => setNumberOfCampuses(parseInt(e.target.value) || 1)}
                                            disabled={calculating}
                                            className="number-input"
                                        />
                                        <p className="hint-text">
                                            (1 to {maxCampuses} campuses available, including ASU)
                                        </p>
                                    </>
                                )}
                            </div>

                            <button 
                                type="submit" 
                                disabled={loading || calculating}
                                className="submit-button"
                            >
                                {calculating ? "Calculating Route..." : "Calculate Tour"}
                            </button>
                        </form>
                    </div>
                ) : (
                    <div className="tour-result-container">
                        <div className="success-message">
                            Tour calculated for {name}!
                        </div>
                        
                        <div className="tour-summary">
                            <h2>Your Optimal Tour Route</h2>
                            <p className="tour-stats">
                                <strong>Total Distance:</strong> {tourResult.totalDistance.toFixed(2)} miles<br/>
                                <strong>Campuses to Visit:</strong> {tourResult.campusCount}
                            </p>
                        </div>
                        
                        <div className="tour-route">
                            <h3>Route Details:</h3>
                            <ol className="route-list">
                                {tourResult.route.map((stop, index) => (
                                    <li key={index} className="route-stop">
                                        <div className="stop-campus">{stop.campus}</div>
                                        {stop.distanceFromPrevious > 0 && (
                                            <div className="stop-distance">
                                                {stop.distanceFromPrevious.toFixed(2)} miles from previous stop
                                            </div>
                                        )}
                                    </li>
                                ))}
                            </ol>
                        </div>
                        
                        <div className="souvenir-section">
                            <h2>Purchase Souvenirs</h2>
                            {error && <div className="error-message">{error}</div>}
                            
                            {loadingSouvenirs ? (
                                <p>Loading souvenirs...</p>
                            ) : (
                                <>
                                    {tourResult.route.map((stop, campusIndex) => {
                                        const campusSouvenirs = souvenirs[stop.campus] || [];
                                        if (campusSouvenirs.length === 0) return null;
                                        
                                        return (
                                            <div key={campusIndex} className="campus-souvenirs">
                                                <h3>Campus: {stop.campus}</h3>
                                                <div className="souvenir-list">
                                                    {campusSouvenirs.map((souvenir, idx) => (
                                                        <div key={idx} className="souvenir-item">
                                                            <span className="souvenir-name">
                                                                {souvenir.name}
                                                            </span>
                                                            <span className="souvenir-price">
                                                                ${souvenir.price.toFixed(2)}
                                                            </span>
                                                            <input
                                                                type="number"
                                                                min="0"
                                                                value={purchases[stop.campus]?.[souvenir.name] || 0}
                                                                onChange={(e) => handleQuantityChange(stop.campus, souvenir.name, e.target.value)}
                                                                className="quantity-input"
                                                                placeholder="Qty"
                                                            />
                                                        </div>
                                                    ))}
                                                </div>
                                                <div className="campus-total">
                                                    <strong>Campus Total: ${calculateCampusTotal(stop.campus).toFixed(2)}</strong>
                                                </div>
                                            </div>
                                        );
                                    })}
                                    
                                    <div className="grand-total">
                                        <h3>Grand Total: ${calculateGrandTotal().toFixed(2)}</h3>
                                    </div>
                                    
                                    <button 
                                        onClick={handleSavePurchases}
                                        className="submit-button"
                                        disabled={savingPurchases || calculateGrandTotal() === 0}
                                    >
                                        {savingPurchases ? "Saving..." : "Complete Purchase"}
                                    </button>
                                </>
                            )}
                        </div>
                        
                        <button 
                            onClick={handleReset}
                            className="submit-button secondary-button"
                        >
                            Plan Another Tour
                        </button>
                    </div>
                )}
            </main>
        </>
    );
}
