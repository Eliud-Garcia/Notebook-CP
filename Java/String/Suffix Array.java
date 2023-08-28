 

//primero llamar a init()
//luego match()


static class suffixArray {

        int K = 26; // tamaño del alfabeto
        int SA[];
        int occ[][];
        int count[];
        int n;

        void init(String s) {
            s += "$";
            n = s.length();
            SA = computeSuffixArray(s);
            occ = new int[n + 1][K];
            count = new int[K + 1];
            for (int i = 0; i < n; i++) {
                int t = s.charAt(SA[i] == 0 ? n - 1 : SA[i] - 1) - 'a';
                if (t >= 0) {
                    occ[i][t]++;
                    count[t + 1]++;
                }
                System.arraycopy(occ[i], 0, occ[i + 1], 0, K);
            }
            for (int i = 1; i < count.length; i++) {
                count[i] += count[i - 1];
            }
        }

        // Retorna indices (start, end) -> de cada aparicion de w en s
        // Si no se necesita un minimo de ocurrencias colocar minOcc = 0

        int[] match(String w, int minOcc) {
            int begin = 0, end = n - 1;
            // List<pair<Integer, Integer>> matches = new ArrayList<>();

            for (int j = w.length() - 1; end - begin + 1 >= minOcc && j >= 0; j--) {
                char c = w.charAt(j);
                int let = c - 'a';
                int nbegin = count[let] + (begin == 0 ? 0 : occ[begin - 1][let]) + 1;
                int nend = count[let] + occ[end][let];
                begin = nbegin;
                end = nend;
            }
            if (end - begin + 1 < minOcc) {
                return new int[] { -1 };
            }

            int t[] = new int[end - begin + 1];
            // se agregan los rangos en que aparece w en s
            for (int j = begin; j <= end; j++) {
                // int inicio = SA[j];
                // int fin = SA[j] + w.length() - 1;
                // matches.add(new pair<>(inicio, fin));
                t[j - begin] = SA[j];
            }
            // ordenar en caso de ser necesario o recorrer en reversa la lista

            Arrays.sort(t);
            return t;

        }

        int[] computeSuffixArray(CharSequence S) {
            int n = S.length();
            int[] sa = IntStream.range(0, n).mapToObj(i -> n - 1 - i)
                    .sorted((a, b) -> Character.compare(S.charAt(a), S.charAt(b))).mapToInt(Integer::intValue)
                    .toArray();
            int[] classes = S.chars().toArray();
            for (int len = 1; len < n; len *= 2) {
                int[] c = classes.clone();
                for (int i = 0; i < n; i++) {
                    classes[sa[i]] = i > 0 && c[sa[i - 1]] == c[sa[i]] && sa[i - 1] + len < n
                            && c[sa[i - 1] + len / 2] == c[sa[i] + len / 2] ? classes[sa[i - 1]] : i;
                }
                int[] cnt = IntStream.range(0, n).toArray();
                int[] s = sa.clone();
                for (int i = 0; i < n; i++) {
                    int s1 = s[i] - len;
                    if (s1 >= 0)
                        sa[cnt[classes[s1]]++] = s1;
                }
            }
            return sa;
        }

    }