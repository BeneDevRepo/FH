package sample;

import java.util.ArrayList;
import java.util.List;
import java.util.Random;

/**
 * Dummy-Imlementierung von {@link Sample}.
 *
 * Diese Implementierung erstellt eine Kopie der Menge, zieht die
 * erforderlichen Elemente aud der Kopie-Menge. Die Elemente werden dabei aus
 * der Menge entfernt.
 *
 * @author M. Faulstich
 */
public class DummySample<T> implements Sample<T>
{

   public static Random rand = new Random(System.currentTimeMillis());

   /**
    * Dummy-Implementierung.
    * 
    * @see aufgabe7.Sample#sample(java.util.List, int)
    */
   @Override
   public List<T> sample(List<T> sample, int draws)
   {
      if (sample == null)
      {
         throw new IllegalArgumentException("Illegal null sample.");
      }
      if (draws < 0)
      {
         throw new IllegalArgumentException(
               "Illegal negative amount of elements should be drawn.");
      }
      if (draws > sample.size())
      {
         throw new IllegalArgumentException(String.format(
               "List contains %d elements, %d Elements should be drawn.",
               sample.size(), draws));
      }
      List<T> result = new ArrayList<T>();
      List<T> copy = new ArrayList<T>(sample);
      for (int i = 0; i < draws; i++)
      {
         int idx = rand.nextInt(copy.size());
         result.add(copy.remove(idx));
      }
      return result;
   }

}
