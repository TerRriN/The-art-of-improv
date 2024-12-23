module Kcolour where
type Vertex = Int
type Edge = (Vertex,Vertex)
type Graph = [Edge]
type Colour = (Vertex, Char)

{- containsVertex v graph
    Check if a vertex exists in a graph
    PRE: The graph is a list of tuples where each tuple represents an edge between two vertices
    RETURNS: True if the vertex exists in the graph, otherwise False
    VARIANT: length graph
    EXAMPLES:   containsVertex 1 [(1, 2), (2, 3), (3, 1)] == True
                containsVertex 4 [(1, 2), (2, 3), (3, 1)] == False
-}
containsVertex :: Vertex -> Graph -> Bool
containsVertex v = any (\(x,y) -> x == v || y == v)

{- addVertex v graph
    Adds a vertex to a graph
    PRE: The graph is a list of tuples where each tuple represents an edge between two vertices
    RETURNS: A new graph with the added vertex
    VARIANT: length graph
    EXAMPLES:   addVertex 1 [(1, 2), (2, 3), (3, 1)] == [(1, 2), (2, 3), (3, 1)]
                addVertex 4 [(1, 2), (2, 3), (3, 1)] == [(1, 2), (2, 3), (3, 1), (4, 4)]
-}
addVertex :: Vertex -> Graph -> Graph
addVertex v g = if containsVertex v g then g else g ++ [(v,v)]

{- addEdge ver1 ver2 graph
    Adds an undirected edge between two vertices in a graph
    PRE: The graph is a list of tuples where each tuple represents an edge between two vertices
    RETURNS: A new graph with the added edge
    VARIANT: length graph
    EXAMPLES:   addEdge 1 2 [(2, 3), (3, 1)]            == [(1, 2), (2, 3), (3, 1)]
                addEdge 1 2 [(1, 2), (2, 3), (3, 1)]    == [(1, 2), (2, 3), (3, 1)]
-}
addEdge :: Vertex -> Vertex -> Graph -> Graph
addEdge a b edges
    | exist = edges
    | otherwise = edges ++ [(a,b)]
    where exist = (a,b) `elem` edges || (b,a) `elem` edges --Check if edge already exist

{- getAllAdjacentVertices v graph adjacentV
    Get all adjacent vertices from a given vertex
    PRE: The graph is a list of tuples where each tuple represents an edge between two vertices
    RETURNS: A list of vertices that are adjacent to the given vertex
    VARIANT: length graph
    EXAMPLES:   getAllAdjacentVertices 1 [(1, 2), (2, 3), (3, 1)] [] == [2,3]
                getAllAdjacentVertices 2 [(1, 2), (2, 3), (3, 1)] [] == [1,3]
-}
getAllAdjacentVertices :: Vertex -> Graph -> [Vertex] -> [Vertex]
getAllAdjacentVertices v [] adjacentV = adjacentV
getAllAdjacentVertices v ((a,b):xs) adjacentV
    | v == a = getAllAdjacentVertices v xs (adjacentV ++ [b])
    | v == b = getAllAdjacentVertices v xs (adjacentV ++ [a])
    | otherwise = getAllAdjacentVertices v xs adjacentV

{- colourVertex v colour
    Assigns a colour to a vertex
    PRE: The colour is a number between 1 and 26
    RETURNS: A tuple where the first element is the vertex and the second is the colour assigned to it
    VARIANT: 
    EXAMPLES:   colourVertex 1 1 == (1, 'a')
                colourVertex 2 3 == (2, 'c')
-}
colourVertex :: Vertex -> Int -> Colour
colourVertex v colour = (v, ['a'..'z'] !! (colour-1))

{- getColour v colours
    Get the colour assigned to a vertex
    PRE: The colours is a list of tuples where each tuple represents a vertex and the colour assigned to it
    RETURNS: The colour assigned to the vertex
    VARIANT: length colours
    EXAMPLES:   getColour 1 [(1, 'a'), (2, 'b'), (3, 'c')] == Just (1, 'a')
                getColour 4 [(1, 'a'), (2, 'b'), (3, 'c')] == Nothing
-}
getColour :: Vertex -> [Colour] -> Maybe Colour
getColour _ [] = Nothing
getColour v ((x,y):xs)
    | v == x = Just (x,y)
    | otherwise = getColour v xs

{- getAllAdjacentColours vertices colours
    Get all adjacent colours from a list of vertices
    PRE: The colours is a list of tuples where each tuple represents a vertex and the colour assigned to it
    RETURNS: A list of colours that are adjacent to the given vertices
    VARIANT: length vertices
    EXAMPLES:   getAllAdjacentColours [1,2,3] [(1, 'a'), (2, 'b'), (3, 'c')]    == ['a','b','c']
                getAllAdjacentColours [1,2,3] [(1, 'a'), (2, 'b')]              == ['a','b']
-}
getAllAdjacentColours :: [Vertex] -> [Colour] -> [Colour]
getAllAdjacentColours [] colours = colours
getAllAdjacentColours (x:xs) colours = 
    case getColour x colours of
        Just c -> getAllAdjacentColours xs (c : colours)
        Nothing -> getAllAdjacentColours xs colours

{- colourExists colours colour
    Check if a colour exists in a list of colours
    PRE: The colours is a list of tuples where each tuple represents a vertex and the colour assigned to it
    RETURNS: True if the colour exists in the list, otherwise False
    VARIANT: length colours
    EXAMPLES:   colourExists [(1, 'a'), (2, 'b'), (3, 'c')] (1, 'a') == True
                colourExists [(1, 'a'), (2, 'b'), (3, 'c')] (4, 'd') == False

-}
colourExists :: [Colour] -> Colour -> Bool
colourExists [] _ = False
colourExists ((_,x):xs) c = x == y || colourExists xs c
    where y = snd c

{- kcolour' graph nColours colouredVertices
    Colour a graph with a given number of colours
    PRE: The graph is a list of tuples where each tuple represents an edge between two vertices
    RETURNS: A list of tuples where the first element is the vertex label and the second is the colour assigned to it
    VARIANT: length graph
    EXAMPLES:   kcolour' [(1, 2), (2, 3), (3, 1)] 3 [] == Just [(1, 'a'), (2, 'b'), (3, 'c')]
                kcolour' [(1, 2), (2, 3), (3, 1)] 2 [] == Nothing
-}
kcolour' :: Graph -> Int -> [Colour] -> Maybe [Colour]
kcolour' [] _ colVer = Just colVer  -- If graph is empty, we have successfully colored all vertices
kcolour' ((x, y):xs) nCol colVer =
    let adjVer = getAllAdjacentVertices x ((x, y):xs) []
        adjCol = getAllAdjacentColours adjVer colVer
    in tryColours x nCol adjCol colVer xs

{- tryColours vertex nColours adjacentColours colouredVertices restGraph
    Try to colour a vertex with a given number of colours
    PRE: The graph is a list of tuples where each tuple represents an edge between two vertices
    RETURNS: A list of tuples where the first element is the vertex label and the second is the colour assigned to it
    VARIANT: length restGraph
    EXAMPLES:   tryColours 1 3 ['a','b'] [(2, 'b')] [(1, 2), (2, 3), (3, 1)] == Just [(1, 'c'), (2, 'b')]
                tryColours 1 3 ['a','b'] [(2, 'b')] [(1, 2), (2, 3), (3, 1)] == Nothing
-}
tryColours :: Vertex -> Int -> [Colour] -> [Colour] -> Graph -> Maybe [Colour]
tryColours v nCol adjCol colVer restGraph = tryColour 1 -- Start trying colors from 1 to nCol
  where
    tryColour :: Int -> Maybe [Colour]  -- Try to color the vertex with a given color
    tryColour colorNum                  -- Try to color the vertex with a given color
      | colorNum > nCol = Nothing     -- No colors left to try, trigger backtracking
      | otherwise =
          let color = colourVertex v colorNum   -- Set the color
          in if colourExists adjCol color       -- Check if the color is valid
             then tryColour (colorNum + 1)      -- Conflict, try the next color
             else kcolour' restGraph nCol (colVer ++ [color])  -- No conflict, proceed with this color

{- kcolour graph nCol
    Colour a graph with a given number of colours
    PRE: The graph is a list of tuples where each tuple represents an edge between two vertices
    RETURNS: A list of tuples where the first element is the vertex label and the second is the colour assigned to it
    VARIANT: length graph
    EXAMPLES:   kcolour [(1, 2), (2, 3), (3, 1)] 3 == Just [(1, 'a'), (2, 'b'), (3, 'c')]
                kcolour [(1, 2), (2, 3), (3, 1)] 2 == Nothing
-}
kcolour :: Graph -> Int -> Maybe [Colour]
kcolour graph nCol = kcolour' graph nCol []