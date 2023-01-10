package sample;

import java.util.List;

/**
 * Ziehung einer Anzahl von Elementen aus einer Liste.
 * 
 * Ein Element kann nur 1x gezogen werden.
 *
 * @author M. Faulstich
 */
public interface Sample<T> {
   /**
    * Zieht eine Anzahl von draws Elementen T aus einer gegebenen Menge sample.
    *
    * @param sample Menge (Liste) mit Elementen, aus der gezogen wird. Darf nicht <code>null</code> sein.
    * @param draws Anzahl der Elemente, die aus sample gezogen werden soll. Gültiger Wertebereich: 0...Anzahl in sample.
    * @return Gezogene draws Elemente aus sample. Mindestens eine leere Liste.
    * @throws IllegalArgumentException bei sample == <code>null</code> oder draws auperhalb des gültigen Wertebereichs.
    */
   public List<T> sample(List<T> sample, int draws);
}
