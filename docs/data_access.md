# Data Access Architecture

Dealing with multiple raster files at once presents many opportunities for unsafe
and messy code. openGCAS attempts to deal with this by establishing four levels of data
access and designating areas of memory where data can be safely queried at any point during runtime.

- Level 0 "Raster Query System (RQS)"
  - Lowest level interface between datafiles and application
  - `RasterQuery` class
    - Processing `data/` root directory
    - Converting lat-long `llPoint` to `nPoint` on raster
    - Handles errant lat-long values safely
    - Allocating memory to load nine rasters on the heap `m_rasterCallOrder`
- Level 1 "Map Manager"
  - Querying data from RQS
  - Filling RQS memory with altitude data
  - Preparing to update `m_rasterCallOrder`
  - Create small stack array with interpolated altitudes for quick access
- 