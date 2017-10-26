package
{
 import flash.display.Sprite;
 import flash.display.StageAlign;
 import flash.display.StageScaleMode;
 import starling.core.Starling;
 
 [SWF(width="800",height="600",frameRate="60",backgroundColor="#FFFFFF")]
 
 public class OsmosMain extends Sprite
 {
  private var mStarling:Starling;
  
  public function OsmosMain()
  {
   
   stage.align = StageAlign.TOP_LEFT;
   stage.scaleMode = StageScaleMode.NO_SCALE;
   // create our Starling instance
   mStarling = new Starling(Main, stage);
   // set anti-aliasing (higher the better quality but slower performance)
   mStarling.antiAliasing = 1;
   // start it!
   mStarling.start();
  }
 }
}