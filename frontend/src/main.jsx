import { createRoot } from 'react-dom/client'
import { BrowserRouter, Routes, Route } from 'react-router'
import Home from './pages/Home/Home.jsx'
import Tours from './pages/Tours/Tours.jsx'
import SaddlebackStart from './pages/Tours/TourOptions/SaddlebackStart/SaddlebackStart.jsx'
import ASUStart from './pages/Tours/TourOptions/ASUStart/ASUStart.jsx'
import UCIStart from './pages/Tours/TourOptions/UCIStart/UCIStart.jsx'
import CustomTour from './pages/Tours/TourOptions/CustomTour/CustomTour.jsx'
import Admin from './pages/Admin/Admin.jsx'
import Campuses from './pages/Campuses/Campuses.jsx'
import './index.css'

createRoot(document.getElementById('root')).render(
    <BrowserRouter>
        <Routes>
            {/* Routes for all pages in program. Each component represents a page */}
            <Route path='/' element={<Home />} />
            <Route path='/tour' element={<Tours />} />
            <Route path='/saddleback-start' element={<SaddlebackStart />} />
            <Route path='/asu-start' element={<ASUStart />} />
            <Route path='/uci-start' element={<UCIStart />} />
            <Route path='/custom-tour' element={<CustomTour />} />
            <Route path='/admin' element={<Admin />} />
            <Route path='/campuses' element={<Campuses />} />
        </Routes>
    </BrowserRouter>
)
