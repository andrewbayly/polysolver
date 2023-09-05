/**
 BUILD and RUN: 
 
 1. cd /Users/andrew/projects/git/PolyominoSolver/test-addon
 2. npm run build
 3. node index.js
 
**/

//index.js
const testAddon = require('./build/Release/testaddon.node')

function main(){ 

  console.log('STARTING...')

  var START = new Date()

  var result = testAddon.solve('[15, 15, 0, 0, 0, 1, 0, 2, 0, 3, 1, 1]')
//  var result = testAddon.solve('[24, 23, 0, 0, 0, 1, 0, 2, 0, 3, 0, 4, 1, 1]')
  
  console.log("RESULT=\n"+ result)

  var END = new Date()

  console.log('ELAPSED: ', END - START)

}

//searches for the smallest rectangle for this polyomino
function findRectangle(poly, odd){ 

  console.log('STARTING...')
  var START = new Date()

  const order = poly.length / 2

  poly.unshift(0)
  poly.unshift(0)
  
  p = 1
  found = false
  
  while(!found){ 
    var size = order * p
    for(var k = 1; k <= Math.sqrt(size); k++){ 
      if(size % k == 0){ 
        var j = size / k
          
        //check rectangle j, k: 
        poly[0] = j
        poly[1] = k
        
        console.log('checking: ', p, j, k)
        
        var result = testAddon.solve(JSON.stringify(poly))
        
        if(result != '[]'){ 
          found = true
          console.log(result)
        }  
      }
    }
    if(odd){ 
      p += 2
    }
    else{ 
      p += 1
    }
  }
 
  var END = new Date()
  console.log('ELAPSED: ', END - START)
}

/*
findRectangle([0,0, 0,1, 0,2, 0,3, 
                    1,1], false) //5ms
*/

//findRectangle([0, 0, 0, 1, 1, 1], true) //5ms

/**
findRectangle([0,0, 0,1, 0,2, 0,3, 0,4, 
                    1,1], false) // 80s
**/

/*
findRectangle([0,0, 0,1, 0,2, 0,3, 0,4, 
                    1,1, 1,2], false) //22s
*/

/*
findRectangle([0,0, 0,1, 0,2, 
               1,0, 1,1, 
               2,0, 2,1, 
                    3,1], false) //reached 102 after approx 4 hours.
*/

/*
findRectangle([     0,1, 0,2, 0,3, 
               1,0, 
               2,0], false) //6.4mins
*/

/*
findRectangle([0,0, 0,1, 0,2, 
               1,0, 1,1, 1,2, 
               2,0, 2,1, 2,2, 
               3,0, 3,1, 3,2, 
                         4,2, 
                         5,2], true)
*/

/*
findRectangle([0,0, 0,1, 0,2, 0,3, 0,4,  
               1,0, 1,1, 1,2, 1,3, 1,4,  
                    2,1, 2,2, 
                    3,1, 3,2], true)
*/

//findRectangle([0, 0, 0, 1, 1, 0, 1, 1, 2, 2], false) ??

//findRectangle([0, 0, 0, 1, 1, 3, 2, 3], false) //4ms

//findRectangle([0, 0, 0, 1, 0, 3, 1, 3], false) //4ms



/*
findRectangle([    0,1, 0,2, 0,3, 0,4, 
               1,0, 
               2,0], false) //reached 98
*/

/*
findRectangle([    0,1, 0,2, 0,3, 0,4, 0,5, 
               1,0, 
               2,0], false) //reached 92
*/

/**
findRectangle([    0,1, 0,2, 0,3, 
               1,0, 
               2,0, 
               3,0 
               ], false) //Impossible?
**/

/**
findRectangle([         0,2, 0,3, 
               1,0, 
               2,0 
               ], false) //easy! 1ms 
**/

/**
findRectangle([         0,2, 0,3, 0,4, 
               1,0, 
               2,0 
               ], false) //easy! 
**/

/**
findRectangle([         0,2, 0,3, 0,4, 0,5, 
               1,0, 
               2,0 
               ], false)  
**/

/*
findRectangle([0,0, 0,1, 0,2, 0,3, 0,4, 
               1,0, 1,1, 1,2, 1,3, 1,4,
               2,0, 2,1  
              ], false)
*/

/**
findRectangle([0,0, 0,1, 0,2, 0,3, 
               1,0, 1,1, 1,2, 1,3,
               2,0, 2,1, 2,2  
              ], false) //700ms  
**/

/**
findRectangle([0,0, 0,1, 0,2, 0,3, 0,4, 0,5, 
               1,0, 1,1, 1,2, 
               2,0, 2,1, 2,2, 
               ], true) //reached 35
**/

/**
findRectangle([0,0, 0,1, 0,2, 0,3, 0,4, 
               1,0, 1,1, 1,2, 1,3
               ], true) 
**/

/**
findRectangle([0,0, 0,1, 0,2, 0,3, 
               1,0, 1,1, 1,2, 
                         2,2  
              ], false) 
**/

findRectangle([0,0, 0,1, 0,2, 0,3, 0,4, 0,5, 0,6, 0,7, 
               1,0, 1,1, 1,2, 1,3, 1,4, 1,5, 1,6, 1,7, 1,8,
               2,0, 2,1, 
               3,0  
              ], false)




module.exports = testAddon


