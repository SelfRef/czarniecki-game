/*  arc_of_sight()
**  by masterofkings
**
**  General form: arc_of_sight(xPos,yPos,myDir,myRange,myAngle,objToLookFor,objToIgnore)
**
**  Argument list:
**    argument0 (xPos):         the x position to look from
**    argument1 (yPos):         the y position to look from
**    argument2 (myDir):        the checking object's current direction
**    argument3 (myRange):      the maximum distance the arc of sight can see
**    argument4 (myAngle):      the angle of the arc of sight
**    argument5 (objToLookFor): the object to look for
**    argument6 (objToIgnore):  the object which gets in the way of sight (such as walls)
**
**  Usage: Returns the ID of the the nearest object within the arc of sight, which can be seen (there's nothing in the way)
**         or 'noone' (-4) if there is no instance that meets the above criteria.
*/
var foundObject, range, dis; //variables for the calculations
foundObject=noone; //variable for holding the id of found instance (if any)
range=argument3; //variable for holding the distance to an object

with (argument5) //for looping through all instances of the object you wish to find
{
    dis=point_distance(argument0,argument1,x,y) //find the distance from the looking object to the object to look for
    if (dis < range) //if the distance to the instance is closer
    {
        if abs((((((argument2 - point_direction(argument0,argument1,x,y)) mod 360) + 540) mod 360) - 180)) < (argument4/2) //if the object is within arc of sight
        {  
            if (collision_line(argument0,argument1,x,y,argument6,true,false) == noone) //if there's nothing in the way
            {
                foundObject=id //the id of the found object is stored
                range=dis //the distance to that object becomes the new myRange distance
            }
        }
    }
}
return foundObject; //returns id of foundObject (either id of an instance or noone [-4])
